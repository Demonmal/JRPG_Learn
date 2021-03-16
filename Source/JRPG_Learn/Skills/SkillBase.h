// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillUsedSignature)

class AJRPG_PlayerController;

UCLASS()
class JRPG_LEARN_API ASkillBase : public AActor
{
	GENERATED_BODY()
public:
	FOnSkillUsedSignature OnSkillUsed;

	UFUNCTION(BlueprintCallable)
	void UseSkill();

	FORCEINLINE int GetManaRequired() const {return ManaRequired;}

	FORCEINLINE void SetPlayerController (AJRPG_PlayerController* Controller) {PlayerController = Controller;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText ShortDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText FullDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ManaRequired;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanUseInExplore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AJRPG_PlayerController* PlayerController;
};
