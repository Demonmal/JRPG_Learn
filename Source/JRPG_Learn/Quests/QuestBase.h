// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/DataTypes.h"
#include "QuestBase.generated.h"

UCLASS()
class JRPG_LEARN_API AQuestBase : public AActor
{
	GENERATED_BODY()
	
	public:

	FORCEINLINE bool IsRepeatable() const {return bIsRepeatable;}
	FORCEINLINE int GetStepsCount() const {return StepsCount;}
	FORCEINLINE bool GetShowNotification() const {return bShowNotification;}

	UPROPERTY()
	TArray<TSubclassOf<AQuestBase>> RequiredQuests;

	private:

	UPROPERTY()
	FText Name;
	UPROPERTY()
	FText Description;
	UPROPERTY()
	int StepsCount;
	UPROPERTY()
	bool bIsRepeatable;
	UPROPERTY()
	bool bShowNotification;
	UPROPERTY()
	FQuestSounds QuestSounds;
	UPROPERTY()
	FQuestRewards QuestRewards;
	UPROPERTY()
	FText QuestUnavailableText;
	UPROPERTY()
	FText NewQuestText;
	UPROPERTY()
	FText QuestInProgressText;
	UPROPERTY()
	FText QuestReadyToDeliverText;
	UPROPERTY()
	FText QuestAlreadyCompletedText;
};
