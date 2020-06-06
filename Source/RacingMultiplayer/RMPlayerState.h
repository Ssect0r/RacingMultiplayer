// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnrealNetwork.h"
#include "RMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RACINGMULTIPLAYER_API ARMPlayerState : public APlayerState
{
	GENERATED_BODY()

		ARMPlayerState();

	UPROPERTY(Replicated, Category = Gameplay, EditAnywhere)
	bool bIsReady = false;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//UFUNCTION(NetMulticast , reliable)
	void ReadyCheck();

	//UFUNCTION(Server, unreliable)
	//void Server_ReadyCheck();

	UFUNCTION()
	FORCEINLINE bool GetbIsReady() { return bIsReady; }

	UFUNCTION()
	void SetbIsReady(bool flag);

	UFUNCTION(server, reliable, WithValidation)
	void Server_SetbIsReady(bool flag);
};
