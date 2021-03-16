// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestLogic.generated.h"

class AJRPG_PlayerController;
class AQuestBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewQuestReceivedSignature, TSubclassOf<AQuestBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestInProgressSignature, TSubclassOf<AQuestBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestReadyToDeliverSignature, TSubclassOf<AQuestBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestAlreadyCompletedSignature, TSubclassOf<AQuestBase>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestUnavailableSignature, TSubclassOf<AQuestBase>)

UCLASS()
class JRPG_LEARN_API AQuestLogic : public AActor
{
	GENERATED_BODY()
	
public:

	FOnNewQuestReceivedSignature OnNewQuestReceived;
	FOnQuestInProgressSignature OnQuestInProgress;
	FOnQuestReadyToDeliverSignature OnQuestReadyToDeliver;
	FOnQuestAlreadyCompletedSignature OnQuestAlreadyCompleted;
	FOnQuestUnavailableSignature OnQuestUnavailable;

	void CheckQuestStatus(AJRPG_PlayerController*);
	TSubclassOf<AQuestBase> GetCurrentQuest();

private:

	UPROPERTY()
	TArray<TSubclassOf<AQuestBase>> Quests;

	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;

};
