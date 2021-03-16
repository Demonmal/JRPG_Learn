// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "ItemObtainDialogue.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnConfirmedSignature)

class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;
class APlayerUnitBase;
class UOverlay;
class UImage;
class UItemButton;
class UVerticalBox;
class UTextBlock;

UCLASS()
class JRPG_LEARN_API UItemObtainDialogue : public UUIBase
{
	GENERATED_BODY()
	
	public:

	UItemObtainDialogue();

	FOnConfirmedSignature OnConfirmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DialogueText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DialogueTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemButton> ItemButtonClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* PlayerUnitRewardOverlay;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* GoldRewardOverlay;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* UnitImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* UnitName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* GoldRewardAmount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* ItemList;

	UPROPERTY()
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY()
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItems;
	UPROPERTY()
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItems;
	UPROPERTY()
	TMap<TSubclassOf<AEquipmentBase>, int> Equipment;
	UPROPERTY()
	int Gold;

	private:

	UFUNCTION(BlueprintCallable)
	void CallOnConfirmed();

	UFUNCTION(BlueprintCallable)
	void ShowObtainedPlayerUnit();
	UFUNCTION(BlueprintCallable)
	void ShowObtainedUsableItems();
	UFUNCTION(BlueprintCallable)
	void ShowObtainedMiscItems();
	UFUNCTION(BlueprintCallable)
	void ShowObtainedEquipment();
	UFUNCTION(BlueprintCallable)
	void ShowObtainedGold();
};
