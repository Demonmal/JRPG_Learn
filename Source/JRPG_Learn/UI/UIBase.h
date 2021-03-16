// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBase.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API UUIBase : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void FocusOnThisUI();

	UFUNCTION(BlueprintImplementableEvent )
	void MenuUp();
	UFUNCTION(BlueprintImplementableEvent )
	void MenuDown();
	UFUNCTION(BlueprintImplementableEvent )
	void MenuRight();
	UFUNCTION(BlueprintImplementableEvent )
	void MenuLeft();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent )
	void MenuConfirm();
	UFUNCTION(BlueprintImplementableEvent )
	void MenuCancel();
	UFUNCTION(BlueprintImplementableEvent )
	void MenuDetails();

	UFUNCTION(BlueprintCallable)
	void SetUIInput(bool bIsBlocked);

	UPROPERTY(BlueprintReadWrite)
	bool bIsUIInputBlocked;

	private:

	void ListenInputActions();

	FOnInputAction ListenMenuUp;
	FOnInputAction ListenMenuDown;
	FOnInputAction ListenMenuRight;
	FOnInputAction ListenMenuLeft;
	FOnInputAction ListenMenuConfirm;
	FOnInputAction ListenMenuCancel;
	FOnInputAction ListenMenuDetails;

	UFUNCTION()
	void ListenMenuUpLogic();
	UFUNCTION()
	void ListenMenuDownLogic();
	UFUNCTION()
	void ListenMenuRightLogic();
	UFUNCTION()
	void ListenMenuLeftLogic();
	UFUNCTION()
	void ListenMenuConfirmLogic();
	UFUNCTION()
	void ListenMenuCancelLogic();
	UFUNCTION()
	void ListenMenuDetailsLogic();

};
