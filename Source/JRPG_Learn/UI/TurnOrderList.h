// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnOrderList.generated.h"

class ABattleController;
class AUnitBase;

UCLASS()
class JRPG_LEARN_API UTurnOrderList : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void InitTurnOrderList(ABattleController* Controller);
	UFUNCTION(BlueprintCallable)
	void UpdateTurnOderList(AUnitBase* CurrentUnit);
	UFUNCTION(BlueprintCallable)
	void SetRevivedUnit(AUnitBase* Unit);

	private:

	UPROPERTY()
	TMap<AUnitBase*, float> UnitsWithAT;
	
	TWeakObjectPtr<ABattleController> BattleController;
	TWeakObjectPtr<AUnitBase> RevivedUnit;
};
