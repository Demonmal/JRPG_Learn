// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UIBase.h"
#include "InfoDialogue.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnConfirmedSignature)

UCLASS()
class JRPG_LEARN_API UInfoDialogue : public UUIBase
{
	GENERATED_BODY()

	public:

	UInfoDialogue();

	FOnConfirmedSignature OnConfirmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DialogueText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DialogueTitle;

	UFUNCTION(BlueprintCallable)
	void CallOnConfirmed();
	
};
