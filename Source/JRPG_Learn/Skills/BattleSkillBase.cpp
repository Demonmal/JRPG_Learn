// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleSkillBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Battle/BattleController.h"
#include "../Battle/BattleBase.h"
#include "../Units/UnitBase.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "../Units/EnemyUnits/EnemyUnitBase.h"
#include "TimerManager.h"

void ABattleSkillBase::UseBattleSkillInExplore(TSubclassOf<APlayerUnitBase> TargetUnit)
{
    if (OnSkillUsed.IsBound())
    {
        OnSkillUsed.Broadcast();
    }
}

void ABattleSkillBase::PlaySkillAnimation()
{
    AUnitBase *CurrentAttackingUnit = BattleController->GetCurrentAttackingUnit();
    if (SkillAnimations.Contains(CurrentAttackingUnit->GetClass()))
    {
        UAnimMontage *AnimMontage = SkillAnimations[CurrentAttackingUnit->GetClass()];
        // FScriptDelegate MontageNotifyDelegate;
        // MontageNotifyDelegate.BindUFunction(this, "OnAnimMontageNotifyHandler");
        // CurrentAttackingUnit->GetAnimInstance()->OnPlayMontageNotifyBegin.Add(MontageNotifyDelegate);
        CurrentAttackingUnit->PlayAnimMontage(AnimMontage);
        FTimerDelegate TimerCallback;
        FTimerHandle TimerHandle;
        TimerCallback.BindLambda([&]() {
            OnSkillAnimationEnded();
        });
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, AnimMontage->SequenceLength, false);      
        UseSkillOnNotify();  
    }
    else
    {
        OnSkillAnimationEnded();
    }
}

void ABattleSkillBase::OnAnimMontageNotifyHandler(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload)
{
    UE_LOG(LogTemp, Log, TEXT("ABattleSkillBase::OnAnimMontageNotifyHandler %s"), *NotifyName.ToString())
    if (NotifyName == UseSkillNotifyName)
    {
        UseSkillOnNotify();
    }
}

void ABattleSkillBase::SetBattleController(ABattleController *Controller)
{
    BattleController = Controller;
}

AUnitBase *ABattleSkillBase::GetTargetUnitForEnemy()
{
    ABattleBase *CurrentBattle = BattleController->GetCurrentBattle();
    AUnitBase *TargetUnit;
    if (SkillType == ESkillType::Offensive)
    {
        TArray<APlayerUnitBase *> PlayerUnits = CurrentBattle->GetPlayerUnits();
        TargetUnit = PlayerUnits[UKismetMathLibrary::RandomIntegerInRange(0, PlayerUnits.Num() - 1)];
    }
    else
    {
        TArray<AEnemyUnitBase *> EnemyUnits = CurrentBattle->GetEnemyUnits();
        TargetUnit = EnemyUnits[UKismetMathLibrary::RandomIntegerInRange(0, EnemyUnits.Num() - 1)];
    }
    return TargetUnit;
}