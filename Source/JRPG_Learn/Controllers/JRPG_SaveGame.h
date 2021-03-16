// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Data/DataTypes.h"
#include "JRPG_SaveGame.generated.h"

class APlayerUnitBase;

UCLASS()
class JRPG_LEARN_API UJRPG_SaveGame : public USaveGame
{
	GENERATED_BODY()

	void SaveJRPGPlayerController()
	{

	}

	private:

	UPROPERTY()
	TMap<TSubclassOf<APlayerUnitBase>, FPlayerUnitData> PlayerUnits;
	UPROPERTY()
	TArray<FString> RemovedBattles;
	UPROPERTY()
	TArray<FString> Interactions;
	UPROPERTY()
	FDateTime SaveDate;
	UPROPERTY()
	FName CurrentMapName;
	UPROPERTY()
	TArray<TSubclassOf<APlayerUnitBase>> PartyMembers;
	UPROPERTY()
	int Gold;
};
