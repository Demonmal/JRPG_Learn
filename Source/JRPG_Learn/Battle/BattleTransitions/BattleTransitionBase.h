// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleTransitionBase.generated.h"

DECLARE_MULTICAST_DELEGATE(OnTransitionEndedSignature)

UCLASS()
class JRPG_LEARN_API ABattleTransitionBase : public AActor
{
	GENERATED_BODY()
	
	public:

	OnTransitionEndedSignature OnTransitionEnded;

	void StartTransition();

	protected:

	TWeakObjectPtr<class AJRPG_PlayerController> PlayerController;
	TWeakObjectPtr<class ABattleBase> Battle;
};
