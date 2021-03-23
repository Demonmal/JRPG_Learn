// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/DataTypes.h"
#include "Components/Widget.h"
#include "ActionsUI.generated.h"

class UActionButton;

DECLARE_MULTICAST_DELEGATE_OneParam(OnActionPressedSignature, EActionType)

UCLASS()
class JRPG_LEARN_API UActionsUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	OnActionPressedSignature OnActionPressed;

	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void ShowActionsUI();
	UFUNCTION(BlueprintCallable)
	void HideActionsUI();
	UFUNCTION()
	void OnActionButtonClicked(EActionType Type);

	UPROPERTY(meta = (BindWidgetAnim))
	class UWidgetAnimation* ShowAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* AttackButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* FleeButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* ItemButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* SkillButton;

	UPROPERTY()
	TArray<UActionButton*> AllActionButtons;
};
