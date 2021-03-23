// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSpawnLocation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "BattleBase.h"

void AUnitSpawnLocation::Init(ABattleBase *Battle_l)
{
	Battle = TWeakObjectPtr<ABattleBase>(Battle_l);
	FScriptDelegate OnDestoyedDelegate;
	OnDestoyedDelegate.BindUFunction(this, "OnBattleDestroyedHandler");
	Battle->OnDestroyed.Add(OnDestoyedDelegate);
}

void AUnitSpawnLocation::OnBattleDestroyedHandler(AActor* DestroyedActor)
{
	Destroy();
}