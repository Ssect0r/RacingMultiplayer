// Fill out your copyright notice in the Description page of Project Settings.

#include "RMGameStateBase.h"
#include "RMPlayerState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


ARMGameStateBase::ARMGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARMGameStateBase::ReadyCheck_Implementation()
{
	if (Role != ROLE_Authority)
	{
		return;
	}
	bool bStart = false;
		for (APlayerState* PlayerState : PlayerArray)
		{
			ARMPlayerState* RMPlayerState = Cast<ARMPlayerState>(PlayerState);
			if (RMPlayerState)
			{
				if (RMPlayerState->GetbIsReady())
				{
					bStart = true;
					ActualReadyPlayers++;
				}
				else
				{
					bStart = false;
				}
			}
			else
			{
				bStart = false;
			}
		}
		if (bStart && ActualReadyPlayers>=MinOfPlayers)
		{
			int iterator = 0;
			// Restart Positions
			UE_LOG(LogTemp, Warning, TEXT("RESTART"));
			bStart = false;
			ActualReadyPlayers = 0;
			TArray<AActor*> SpawningPoints;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawningPoints);

			//for (AActor* spawn : SpawningPoints)
			//{
			//	APlayerStart* PlayerStart = Cast<APlayerStart>(spawn);
			//		if(PlayerStart)
			//		{
			//			
			//		}
			//	
			//}

			for (APlayerState* PlayerState : PlayerArray)
			{
				ARMPlayerState* RMPlayerState = Cast<ARMPlayerState>(PlayerState);
				if (RMPlayerState)
				{
					//RMPlayerState->Server_SetbIsReady(false);
					RMPlayerState->GetPawn()->SetActorLocation(SpawningPoints[iterator++]->GetActorLocation());
				}
			}
			iterator = 0;
		}
		ActualReadyPlayers = 0;
		bStart = false;
}

void ARMGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ReadyCheck();
}


//bool ARMGameStateBase::ReadyCheck_Validate()
//{
//	return true;
//}
//
//void ARMGameStateBase::ReadyCheck_Implementation()
//{
//	bool bStart = false;
//	for (APlayerState* PlayerState : PlayerArray)
//	{
//		ARMPlayerState* RMPlayerState = Cast<ARMPlayerState>(PlayerState);
//		if (RMPlayerState)
//		{
//			if (RMPlayerState->GetbIsReady())
//			{
//				bStart = true;
//			}
//			else
//			{
//				bStart = false;
//			}
//		}
//		else
//		{
//			bStart = false;
//		}
//	}
//	if (bStart)
//	{
//		// Restart Positions
//	}
//}


