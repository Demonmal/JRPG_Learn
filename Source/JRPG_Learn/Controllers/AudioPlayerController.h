// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioPlayerController.generated.h"

class USoundCue;

UCLASS()
class JRPG_LEARN_API AAudioPlayerController : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void PlaySoundByTag(FName Tag);

protected: 
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	UDataTable* SoundsTable;
};
