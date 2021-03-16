// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInteractedSignature)

UCLASS()
class JRPG_LEARN_API UInteractionUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	FOnInteractedSignature OnInteracted;

	UFUNCTION(BlueprintCallable)
	void Init(FText InteractionText);

	UFUNCTION(BlueprintCallable)
	void ShowInteractionUI();

	UFUNCTION(BlueprintCallable)
	void HideInteractionUI();

	UPROPERTY(meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowAnimation;
	UPROPERTY(meta = (BindWidget))
	class UActionButton* InteractionButton;
};
