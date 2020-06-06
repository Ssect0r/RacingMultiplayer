// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "GameFramework/Character.h"
#include "UnrealNetwork.h"
#include "RacingMultiplayerPawn.generated.h"


UCLASS(Blueprintable)
class ARacingMultiplayerPawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Replicated, Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ARacingMultiplayerPawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	FVector Acceleration;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	FVector Velocity;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float MaxDegreesPerSecond = 90.f;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float Steering;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float Mass = 1000;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	FVector Force;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	FVector Location;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float Throttle;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float MinTurningRadius = 7.f;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	float MaxDrivingForce = 10000;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	class UBoxComponent * Box;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;

	//UFUNCTION(Server, Reliable, WithValidation)
	UFUNCTION()
	void Server_UpdateRotation(float DeltaSeconds);

	UFUNCTION()
	void LocationFromVelocity(float DeltaSeconds);

	UFUNCTION()
	void ReadyCheck();

	UFUNCTION()
	FVector GetAirResistance();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	FVector GetRollingResistance();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Static names for axis bindings


private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Val);
	void MoveForward(float Val);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Val);
	void MoveRight(float Val);

	UFUNCTION()
	void TurnAtRate(float Rate);


public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

