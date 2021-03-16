// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "ActionsUI.generated.h"

class UActionButton;

UCLASS()
class JRPG_LEARN_API UActionsUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void ShowActionsUI();

	UFUNCTION(BlueprintCallable)
	void HideActionsUI();

	UPROPERTY(meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowAnimation;
};
