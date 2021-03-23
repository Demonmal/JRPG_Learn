// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMovementAnimInstance.h"
#include "../JRPG_CharacterBase.h"

void UBaseMovementAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    CachedCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void UBaseMovementAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    if(CachedCharacter.IsValid())
    {
        Speed = CachedCharacter->GetVelocity().Size();
    }
}