// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBattleAnim.h"
#include "../../Units/UnitBase.h"
#include "Animation/AnimSequenceBase.h"

void UBaseBattleAnim::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    CachedUnit = Cast<AUnitBase>(GetOwningActor());
    bIsDead = false;
    if(CachedUnit.IsValid())
    {
        IdleAnim = Cast<UAnimSequenceBase>(CachedUnit->GetIdleAnim());
        DieAnim = Cast<UAnimSequenceBase>(CachedUnit->GetDieAnim());
        CachedUnit->OnUnitDied.AddLambda([&bIsDeadRef = bIsDead](AUnitBase * DiedUnit) {bIsDeadRef = true;});
    }
}