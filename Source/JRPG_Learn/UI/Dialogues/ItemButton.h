// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButton.generated.h"

class AItemBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemButtonClickedSignature, TSubclassOf<AItemBase>, UItemButton*);

UCLASS()
class JRPG_LEARN_API UItemButton : public UUserWidget
{
	GENERATED_BODY()

	public:

	FOnItemButtonClickedSignature OnItemButtonClicked;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* AmountText;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AItemBase> Item;
	UPROPERTY(BlueprintReadWrite)
	int Amount;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItemDetails(TSubclassOf<AItemBase> CachedItem, int ItemAmount);

	UFUNCTION(BlueprintCallable)
	void SetItemTextAndTexture();
	UFUNCTION(BlueprintCallable)
	void CallOnItemButtonClicked();
	UFUNCTION(BlueprintCallable)
	void SetAmount();
	UFUNCTION(BlueprintCallable)
	void SetEmpty();
};
