// Fill out your copyright notice in the Description page of Project Settings.


#include "RMPlayerController.h"
#include "RMGameStateBase.h"
#include "RMPlayerState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

//bool ARMPlayerController::Server_ReadyCheck_Validate()
//{
//	return true;
//}
//
//void ARMPlayerController::Server_ReadyCheck_Implementation()
//{
//	ARMGameStateBase* GameState = Cast<ARMGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
//	GameState->ReadyCheck();
//}

void ARMPlayerController::RequestReadyCheck(bool flag)
{
	if (Role < ROLE_Authority)
	{
		Server_RequestReadyCheck(flag);
	}
	else
	{
		ARMPlayerState* PS = Cast<ARMPlayerState>(GetPawn()->GetPlayerState());
		//ARMPlayerState* PS = GetPlayerState<ARMPlayerState>();
		if(PS)
		{
				PS->Server_SetbIsReady(flag);

		}
	}
}

void ARMPlayerController::Server_RequestReadyCheck_Implementation(bool flag)
{
	RequestReadyCheck(flag);
}

//bool ARMPlayerController::Server_RequestReadyCheck_Validate()
//{
//	return true;
//}

void ARMPlayerController::BeginPlay()
{
	//if (Role < ROLE_Authority)
	//	Server_ReadyCheck();
}
