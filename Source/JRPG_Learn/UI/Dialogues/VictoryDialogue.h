// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "VictoryDialogue.generated.h"

class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;
class UWidgetAnimation;
class UOverlay;
class UActionButton;

DECLARE_MULTICAST_DELEGATE(FOnContinueClickedSignature);

UCLASS()
class JRPG_LEARN_API UVictoryDialogue : public UUIBase
{
	GENERATED_BODY()

	public:
	UVictoryDialogue();

	FOnContinueClickedSignature OnContinueClicked;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* BP_ActionButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* DropsOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* VictoryAnim;
	 
	UFUNCTION(BlueprintCallable)
	void ShowItemsObtained();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowUsableItemDrops();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowMiscItemDrops();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowEquipmentDrops();
	UFUNCTION(BlueprintCallable)
	void OnConfirm();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ExpReward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GoldReward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItemDrops;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItemDrops;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AEquipmentBase>, int> EquipmentDrops;
};
