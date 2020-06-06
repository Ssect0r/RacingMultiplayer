// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RacingMultiplayerPawn.h"
#include "RacingMultiplayerProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "RMPlayerState.h"
#include "RMPlayerController.h"


ARacingMultiplayerPawn::ARacingMultiplayerPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	//RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	bReplicates = true;
	bReplicateMovement = true;
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetGenerateOverlapEvents(true);
	SetRootComponent(Box);
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	ShipMeshComponent->SetupAttachment(RootComponent);
	
	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}



FVector ARacingMultiplayerPawn::GetAirResistance()
{
	return -1 * Velocity.GetSafeNormal() * Velocity.SizeSquared() * 16;
}

void ARacingMultiplayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ARacingMultiplayerPawn, Velocity);
	DOREPLIFETIME(ARacingMultiplayerPawn, Acceleration);
	DOREPLIFETIME(ARacingMultiplayerPawn, MaxDegreesPerSecond);
	DOREPLIFETIME(ARacingMultiplayerPawn, ShipMeshComponent);
	DOREPLIFETIME(ARacingMultiplayerPawn, Mass);
	DOREPLIFETIME(ARacingMultiplayerPawn, Force);
	DOREPLIFETIME(ARacingMultiplayerPawn, Throttle);
	DOREPLIFETIME(ARacingMultiplayerPawn, MinTurningRadius);
	DOREPLIFETIME(ARacingMultiplayerPawn, MaxDrivingForce);
	DOREPLIFETIME(ARacingMultiplayerPawn, Box);

}

FVector ARacingMultiplayerPawn::GetRollingResistance()
{
	float GravityAcceleration = GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * -GravityAcceleration;
	FVector RollingResistance = -1 * Velocity.GetSafeNormal() * 0.015 * NormalForce;
	return RollingResistance;
}

void ARacingMultiplayerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ARacingMultiplayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARacingMultiplayerPawn::MoveRight);
	PlayerInputComponent->BindAction("Ready", IE_Pressed ,this, &ARacingMultiplayerPawn::ReadyCheck);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARacingMultiplayerPawn::TurnAtRate);
}

void ARacingMultiplayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaSeconds;

	Server_UpdateRotation(DeltaSeconds);
	
	LocationFromVelocity(DeltaSeconds);
	
	Location += Velocity;
	if (Velocity != Velocity.ZeroVector)
	{
		SetActorLocation(Location);
	}
}

void ARacingMultiplayerPawn::Server_UpdateRotation(float DeltaSeconds)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaSeconds;
	float RotationAngle = DeltaLocation / MinTurningRadius * Steering;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	AddActorWorldRotation(RotationDelta);
	Velocity = RotationDelta.RotateVector(Velocity);

	//Server_UpdateRotation_Implementation(DeltaSeconds);
}

//void ARacingMultiplayerPawn::Server_UpdateRotation_Implementation(float DeltaSeconds)
//{
//	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity) * DeltaSeconds;
//	float RotationAngle = DeltaLocation / MinTurningRadius * Steering;
//	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
//	AddActorWorldRotation(RotationDelta);
//	Velocity = RotationDelta.RotateVector(Velocity);
//}
//
//bool ARacingMultiplayerPawn::Server_UpdateRotation_Validate(float DeltaSeconds)
//{
//	return true;
//}

void ARacingMultiplayerPawn::LocationFromVelocity(float DeltaSeconds)
{
	FVector Translation = Velocity * 100 * DeltaSeconds;
	//FHitResult Hit;
	//AddActorWorldOffset(Translation, true, &Hit);
	//if (Hit.IsValidBlockingHit())
	//{
	//	Velocity = FVector::ZeroVector;
	//}
}

void ARacingMultiplayerPawn::ReadyCheck()
{
	ARMPlayerState* RMPlayerState = Cast<ARMPlayerState>(GetPlayerState());
	ARMPlayerController* playerController = Cast<ARMPlayerController>(GetController());
	if (RMPlayerState->GetbIsReady() == false)
	{
		//RMPlayerState->Server_SetbIsReady(true);
		playerController->Server_RequestReadyCheck(true);
	}
	else
	{
		//RMPlayerState->Server_SetbIsReady(false);
		playerController->Server_RequestReadyCheck(false);
	}
	bool test = RMPlayerState->GetbIsReady();
	UE_LOG(LogTemp, Warning, TEXT("Ready = %d"),RMPlayerState->GetbIsReady() ? 1 : 0);
}

//void ARacingMultiplayerPawn::Server_LocationFromVelocity_Implementation(float DeltaSeconds)
//{
//	FVector Translation = Velocity * 100 * DeltaSeconds;
//	FHitResult Hit;
//	AddActorWorldOffset(Translation, true, &Hit);
//	if (Hit.IsValidBlockingHit())
//	{
//		Velocity = FVector::ZeroVector;
//	}
//}

//bool ARacingMultiplayerPawn::Server_LocationFromVelocity_Validate(float DeltaSeconds)
//{
//	return true;
//}

void ARacingMultiplayerPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<ARacingMultiplayerProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ARacingMultiplayerPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void ARacingMultiplayerPawn::ShotTimerExpired()
{
	bCanFire = true;
}

void ARacingMultiplayerPawn::Server_MoveForward_Implementation(float Val)
{
		Throttle = Val;
}

bool ARacingMultiplayerPawn::Server_MoveForward_Validate(float Val)
{
	return FMath::Abs(Val) <= 1;
}

void ARacingMultiplayerPawn::Server_MoveRight_Implementation(float Val)
{
	Steering = Val;
}

bool ARacingMultiplayerPawn::Server_MoveRight_Validate(float Val)
{
	return FMath::Abs(Val) <= 1;
}

void ARacingMultiplayerPawn::MoveForward(float Val)
{
	Throttle = Val;
	Server_MoveForward(Val);
}

void ARacingMultiplayerPawn::MoveRight(float Val)
{
	Steering = Val;
	Server_MoveRight(Val);
	//APlayerController::
}

void ARacingMultiplayerPawn::TurnAtRate(float Rate)
{
	if(Acceleration != FVector::ZeroVector)
	AddControllerYawInput(Rate);
}



