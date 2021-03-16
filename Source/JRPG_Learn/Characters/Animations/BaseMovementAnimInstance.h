// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseMovementAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API UBaseMovementAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	float Speed;

	private:

	TWeakObjectPtr<class AJRPG_CharacterBase> CachedCharacter;
};
