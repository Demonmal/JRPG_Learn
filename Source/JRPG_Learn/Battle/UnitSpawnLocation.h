// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnLocation.generated.h"

class ABattleBase;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class JRPG_LEARN_API AUnitSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:

	void Init(ABattleBase *Battle_l);

protected:

	UFUNCTION()
	void OnBattleDestroyedHandler(AActor* DestroyedActor);

	TWeakObjectPtr<ABattleBase> Battle;

};
