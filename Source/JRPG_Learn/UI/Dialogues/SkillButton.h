// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

class ASkillBase;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillButtonClickedSignature, TSubclassOf<ASkillBase>, USkillButton*);

UCLASS()
class JRPG_LEARN_API USkillButton : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	virtual void NativeConstruct() override;

	FOnSkillButtonClickedSignature OnSkillButtonClicked;

	UFUNCTION(BlueprintCallable)
	void SetSkillTextAndTexture();
	UFUNCTION(BlueprintCallable)
	void SetMPAmount();
	UFUNCTION(BlueprintCallable)
	void CallOnSkillButtonClicked();
	UFUNCTION(BlueprintCallable)
	void PlayMenuNavigationSound();

	UFUNCTION(BlueprintImplementableEvent)
	void Highlight(bool bIsHighlighted);

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ASkillBase> Skill;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* SkillImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SkillName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MPAmount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* ManaOverlay;

	private:

	TWeakObjectPtr<class AAudioPlayerController> AudioPlayerController;
	
};
