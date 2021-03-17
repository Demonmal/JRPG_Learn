// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "SkillUseDialogue.generated.h"

class UWidgetAnimation;
class ABattleSkillBase;
class USkillButton;
class UTextBlock;
class UScrollBox;
class UActionButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillSelectedSignature, TSubclassOf<ABattleSkillBase>);
DECLARE_MULTICAST_DELEGATE(FOnCanceledSignature);

UCLASS()
class JRPG_LEARN_API USkillUseDialogue : public UUIBase
{
	GENERATED_BODY()
	
	public:

	USkillUseDialogue();

	FOnSkillSelectedSignature OnSkillSelected;
	FOnCanceledSignature OnCanceled;

	UFUNCTION(BlueprintCallable)
	void PlayShowSkillsAnim(EUMGSequencePlayMode::Type PlayMode);
	UFUNCTION(BlueprintCallable)
	void PlaySkillActionButtonsAnim(EUMGSequencePlayMode::Type PlayMode);
	UFUNCTION(BlueprintCallable)
	void ShowSkills();
	UFUNCTION(BlueprintCallable)
	void CheckIfSelectedSkillAndHighlightNewOne();
	UFUNCTION(BlueprintCallable)
	void HighlightSelectedSkill();
	UFUNCTION(BlueprintCallable)
	void CallOnSkillSelected(TSubclassOf<ABattleSkillBase> SelectedSkill);
	UFUNCTION(BlueprintCallable)
	void CallOnCanceled();
	UFUNCTION(BlueprintCallable)
	void SetSkills(TArray<TSubclassOf<ABattleSkillBase>> Skills);

	UFUNCTION(BlueprintImplementableEvent)
	void SelectFirstItem();
	UFUNCTION(BlueprintImplementableEvent)
	void Confirm(EActionType ActionType);
	UFUNCTION(BlueprintImplementableEvent)
	void Cancel(EActionType ActionType);

	UFUNCTION()
	void OnSkillButtonClickedHandler(TSubclassOf<ASkillBase> SkillClass, USkillButton* SkillButton);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DialogueTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<USkillButton> SkillButtonClass;

	UPROPERTY(BlueprintReadWrite)
	int SelectedSkillIndex;
	UPROPERTY(BlueprintReadWrite)
	TArray<USkillButton*> SkillButtons;
	UPROPERTY(BlueprintReadWrite)
	USkillButton* HighlightedSkill;	

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowSkillActionButtonsAnim;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowSkillAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* EmptyText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SkillDescriptionText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* SkillList;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* ConfirmButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionButton* CancelButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* SkillDescriptionOverlay;
};
