// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "RMPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class RACINGMULTIPLAYER_API ARMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void RequestReadyCheck(bool flag);

	UFUNCTION(Server, reliable)
	void Server_RequestReadyCheck(bool flag);

	virtual void BeginPlay() override;
};
