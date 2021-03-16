// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestLogic.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "QuestBase.h"
#include "../Data/Enums.h"

void AQuestLogic::CheckQuestStatus(AJRPG_PlayerController *Controller)
{
	PlayerController = Controller;
	TSubclassOf<AQuestBase> CurrentQuest = GetCurrentQuest();
	EQuestStatus QuestStatus = PlayerController->GetQuestStatus(CurrentQuest);
	switch (QuestStatus)
	{
	case EQuestStatus::Unavailable:
		if (OnQuestUnavailable.IsBound())
		{
			OnQuestUnavailable.Broadcast(CurrentQuest);
		}
		break;
	case EQuestStatus::New:
		if (OnNewQuestReceived.IsBound())
		{
			OnNewQuestReceived.Broadcast(CurrentQuest);
		}
		break;
	case EQuestStatus::Active:
		if (OnQuestInProgress.IsBound())
		{
			OnQuestInProgress.Broadcast(CurrentQuest);
		}
		break;
	case EQuestStatus::ReadyToDeliver:
		if (OnQuestReadyToDeliver.IsBound())
		{
			OnQuestReadyToDeliver.Broadcast(CurrentQuest);
		}
		break;
	case EQuestStatus::Completed:
		if (OnQuestAlreadyCompleted.IsBound())
		{
			OnQuestAlreadyCompleted.Broadcast(CurrentQuest);
		}
		break;
	}
}

TSubclassOf<AQuestBase> AQuestLogic::GetCurrentQuest()
{
	TSubclassOf<AQuestBase> CurrentQuest = Quests.Last();
	for (int i{0}; i < Quests.Num(); i++)
	{
		if (!PlayerController->IsQuestCompleted(Quests[i]))
		{
			CurrentQuest = Quests[i];
			break;
		}
		else
		{
			AQuestBase *QuestDefault = Cast<AQuestBase>(Quests[i]->GetDefaultObject());
			if (QuestDefault->IsRepeatable())
			{
				CurrentQuest = Quests[i];
				break;
			}
		}
	}
	return CurrentQuest;
}