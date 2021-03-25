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
class UScrollBox;
class UItemButton;

DECLARE_MULTICAST_DELEGATE(FOnContinueClickedSignature);

UCLASS()
class JRPG_LEARN_API UVictoryDialogue : public UUIBase
{
	GENERATED_BODY()

	public:
	UVictoryDialogue();

	FOnContinueClickedSignature OnContinueClicked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemButton> ItemButtonClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* BP_ActionButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* DropsOverlay;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* VictoryAnim;
	 
	UFUNCTION(BlueprintCallable)
	void ShowItemsObtained();
	UFUNCTION(BlueprintCallable)
	void ShowUsableItemDrops();
	UFUNCTION(BlueprintCallable)
	void ShowMiscItemDrops();
	UFUNCTION(BlueprintCallable)
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
