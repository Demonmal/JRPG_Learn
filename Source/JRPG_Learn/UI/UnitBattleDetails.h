// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitBattleDetails.generated.h"

class AUnitBase;
class UTextBlock;
class UImage;

UCLASS()
class JRPG_LEARN_API UUnitBattleDetails : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable)
	void UpdateUnitDetails(AUnitBase* Unit);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* UnitThumbnail;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* UnitName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* UnitLevel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PhysicalAttack;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Defense;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MagicalAttack;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MagicalDefense;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Hit;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Speed;
};
