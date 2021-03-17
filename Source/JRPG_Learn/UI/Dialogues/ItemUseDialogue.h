// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "ItemUseDialogue.generated.h"

class UWidgetAnimation;
class UTextBlock;
class UScrollBox;
class UActionButton;
class AItemBase;
class AUsableItemBase;
class UItemButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemSelectedSignature, TSubclassOf<AItemBase>);
DECLARE_MULTICAST_DELEGATE(FOnCanceledSignature);

UCLASS()
class JRPG_LEARN_API UItemUseDialogue : public UUIBase
{
	GENERATED_BODY()
	
	public:

	UItemUseDialogue();

	FOnItemSelectedSignature OnItemSelected;
	FOnCanceledSignature OnCanceled;

	UFUNCTION(BlueprintCallable)
	void PlayItemInventoryAnim(EUMGSequencePlayMode::Type PlayMode);
	UFUNCTION(BlueprintCallable)
	void PlayItemActionButtonsAnim(EUMGSequencePlayMode::Type PlayMode);
	UFUNCTION(BlueprintCallable)
	void ShowUsableItemInventory();
	UFUNCTION(BlueprintCallable)
	void CheckIfSelectedItemAndHighlightNewOne();
	UFUNCTION(BlueprintCallable)
	void HighlightSelectedItem();
	UFUNCTION(BlueprintCallable)
	void CallOnItemSelected(TSubclassOf<AItemBase> SelectedItem);
	UFUNCTION(BlueprintCallable)
	void CallOnCanceled();
	UFUNCTION(BlueprintCallable)
	void ShowUsableItems(TMap<TSubclassOf<AUsableItemBase>, int> UsableItems);

	UFUNCTION(BlueprintImplementableEvent)
	void SelectFirstItem();
	UFUNCTION(BlueprintImplementableEvent)
	void Confirm(EActionType ActionType);
	UFUNCTION(BlueprintImplementableEvent)
	void Cancel(EActionType ActionType);

	UFUNCTION()
	void OnItemButtonClickedHandler(TSubclassOf<AItemBase> ItemClass, UItemButton* ItemButton);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DialogueTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemButton> ItemButtonClass;

	UPROPERTY(BlueprintReadWrite)
	TMap<TSubclassOf<AItemBase>, int> Items;
	UPROPERTY(BlueprintReadWrite)
	int SelectedItemIndex;
	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> ItemButtons;
	UPROPERTY(BlueprintReadWrite)
	UItemButton* HighlightedItem;	

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* EmptyText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemDescriptionText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemList;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* ConfirmButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* CancelButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* ItemDescriptionOverlay;

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowItemActionButtons;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowItemInventory;
};
