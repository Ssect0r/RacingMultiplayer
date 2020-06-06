// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RacingMultiplayerGameMode.h"
#include "RacingMultiplayerPawn.h"
#include "Kismet/GameplayStatics.h"

//ARacingMultiplayerGameMode::ARacingMultiplayerGameMode()
//{
//	// set default pawn class to our character class
//	DefaultPawnClass = ARacingMultiplayerPawn::StaticClass();
//	
//}
//
//void ARacingMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	PlayerControllerList.Add(NewPlayer);
//	if (MaxNumPlayers == GetNumPlayers())
//	{
//		
//	} 
//}
//
//
//
//void ARacingMultiplayerGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//
//	MaxNumPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "MaxNumPlayers")));
//}
