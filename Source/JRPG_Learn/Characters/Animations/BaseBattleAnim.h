// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseBattleAnim.generated.h"

class UAnimSequence;

UCLASS()
class JRPG_LEARN_API UBaseBattleAnim : public UAnimInstance
{
	GENERATED_BODY()

	public:

	virtual void NativeBeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequenceBase* IdleAnim;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequenceBase* DieAnim;

	private:

	TWeakObjectPtr<class AUnitBase> CachedUnit;
};
