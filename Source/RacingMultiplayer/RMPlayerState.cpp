// Fill out your copyright notice in the Description page of Project Settings.

#include "RMPlayerState.h"

ARMPlayerState::ARMPlayerState()
{
}

void ARMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ARMPlayerState, bIsReady);
}

void ARMPlayerState::ReadyCheck()
{
}
void ARMPlayerState::SetbIsReady(bool flag)
{
	Server_SetbIsReady(flag);
}

bool ARMPlayerState::Server_SetbIsReady_Validate(bool flag)
{
	return true;
}

void ARMPlayerState::Server_SetbIsReady_Implementation(bool flag)
{
	bIsReady = flag;
}

//
//void ARMPlayerState::Server_ReadyCheck_Implementation()
//{
//	if (!bIsReady)
//	{
//		bIsReady = true;
//	}
//	else
//	{
//		bIsReady = false;
//	}
//}

//bool ARMPlayerState::Server_ReadyCheck_Validate()
//{
//	return true;
//}
