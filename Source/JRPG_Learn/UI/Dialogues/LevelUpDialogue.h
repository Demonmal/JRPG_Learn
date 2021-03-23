// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "../../Data/DataTypes.h"
#include "LevelUpDialogue.generated.h"

class APlayerUnitBase;
class ABattleController;

DECLARE_MULTICAST_DELEGATE(FOnContinueClickedSignature);

UCLASS()
class JRPG_LEARN_API ULevelUpDialogue : public UUIBase
{
	GENERATED_BODY()
	public:
	ULevelUpDialogue();

	FOnContinueClickedSignature OnContinueClicked;

	UFUNCTION(BlueprintCallable)
	void OnConfirm();
	UFUNCTION(BlueprintCallable)
	void ShowNewBattleSkills();
	UFUNCTION(BlueprintCallable)
	void ShowNewExploreSkills();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerUnitData PlayerUnitData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LevelUpAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentLevel;

	TWeakObjectPtr<ABattleController> BattleController;
};
