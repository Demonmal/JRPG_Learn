// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class JRPG_LEARN_API AItemBase : public AActor
{
	GENERATED_BODY()

public:

	FORCEINLINE FText GetName() const {return Name;}
	FORCEINLINE FText GetShortDescription() const {return ShortDescription;}
	FORCEINLINE UTexture2D* GetIcon() const {return Icon;}
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText ShortDescription;
	UPROPERTY(EditDefaultsOnly)
	FText FullDescription;
	UPROPERTY(EditDefaultsOnly)
	int Price;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;
};
