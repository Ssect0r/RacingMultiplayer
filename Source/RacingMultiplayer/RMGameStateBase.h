// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UnrealNetwork.h"
#include "RMGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RACINGMULTIPLAYER_API ARMGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ARMGameStateBase();
	

	int32 ActualReadyPlayers = 0;

public:
	// Function to Increment the Variable
	UFUNCTION(NetMulticast , reliable)
	void ReadyCheck();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
	int32 MinOfPlayers = 3;

	virtual void Tick(float DeltaSeconds) override;
};
