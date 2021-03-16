// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/Enums.h"
#include "ActionButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionButtonClickedSignature, EActionType)

class UImage;

UCLASS()
class JRPG_LEARN_API UActionButton : public UUserWidget
{
	GENERATED_BODY()

	public:

	FOnActionButtonClickedSignature OnActionButtonClicked;

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void CallOnActionButtonClicked(EActionType Type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTextOnRight = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EActionType ActionType = EActionType::Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* ActionImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ActionButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* ActionOverlay;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ActionText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Background;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IconImage;

	private:

	TWeakObjectPtr<class UJRPG_GameInstance> GameInstance;
};
