// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../QuestBase.h"
#include "ItemQuestBase.generated.h"

class AItemBase;

UCLASS()
class JRPG_LEARN_API AItemQuestBase : public AQuestBase
{
	GENERATED_BODY()

	public:

	FORCEINLINE TSubclassOf<AItemBase> GetItemToCollect() const {return ItemToCollect;}
	
	protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemBase> ItemToCollect;
};
