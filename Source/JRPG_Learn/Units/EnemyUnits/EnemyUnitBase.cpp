// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyUnitBase.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Skills/BattleSkillBase.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../Battle/BattleController.h"
#include "../../Battle/BattleBase.h"
#include "../../UI/EnemyUnitUI.h"

AEnemyUnitBase::AEnemyUnitBase() : AUnitBase::AUnitBase()
{
    EnemyUnitWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyUnitWidget"));
    EnemyUnitWidget->SetWidgetSpace(EWidgetSpace::World);
    EnemyUnitWidget->SetDrawAtDesiredSize(true);
    EnemyUnitWidget->SetWindowVisibility(EWindowVisibility::Visible);
    EnemyUnitWidget->SetTickWhenOffscreen(true);
    EnemyUnitWidget->SetVisibility(false);
    EnemyUnitWidget->SetupAttachment(RootComponent);
}

void AEnemyUnitBase::InitUnit(ABattleController *Controller)
{
    AUnitBase::InitUnit(Controller);
    SetHP(MaxHP);
    EnemyUnitWidget->SetVisibility(true);
    EnemyUnitWidget->InitWidget();
    EnemyUnitUI = Cast<UEnemyUnitUI>(EnemyUnitWidget->GetUserWidgetObject());
    EnemyUnitUI->Init(this, BattleController.Get());
    RotateUnitUIToCamera();
}

void AEnemyUnitBase::RotateUnitUIToCamera()
{
    bRotateUnitUIToCamera = true;
}

void AEnemyUnitBase::StopRotateUnitUIToCamera()
{
    bRotateUnitUIToCamera = false;
}

void AEnemyUnitBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(!bRotateUnitUIToCamera)
        return;
    EnemyUnitWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(EnemyUnitWidget->GetComponentLocation(), PlayerController->PlayerCameraManager->GetCameraLocation()));
}

void AEnemyUnitBase::StartTurn()
{
    if(IsUnitDead())
    {
        AUnitBase::StartTurn();
    }
    else
    {
        if(OnTurnStarted.IsBound())
        {
            OnTurnStarted.Broadcast();
        }
        TSubclassOf<ABattleSkillBase> RandomSkill = GetRandomSkill();
        if(UKismetSystemLibrary::IsValidClass(RandomSkill))
        {
            BattleController->EnemyPrepareForSkill(RandomSkill);
        }
        else
        {
            CurrentTarget = GetTargetUnit(true);
            FRotator RotatorToTarget = UKismetMathLibrary::FindLookAtRotation(SkeletalMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
            RotatorToTarget.Yaw -= 90.0f;
            FocusOnTarget(RotatorToTarget);
            CurrentTarget->AdjustAttackerLocation(this);
            BattleController->AdjustCamera(Cast<APlayerUnitBase>(CurrentTarget.Get()), this, false);
            UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);
            BattleController->Attack(CurrentTarget.Get());
        }
    }
}

void AEnemyUnitBase::Die()
{
    AUnitBase::Die();
    if(IsValid(EnemyUnitWidget))
    {
        EnemyUnitWidget->SetVisibility(false);
    }
}

void AEnemyUnitBase::AdjustCameraForSkill(AUnitBase *TargetUnit)
{
    CurrentTarget = TargetUnit;
    APlayerUnitBase *PlayerUnit = Cast<APlayerUnitBase>(TargetUnit);
    if(PlayerUnit)
    {
        BattleController->AdjustCamera(PlayerUnit, this, false);
        FRotator RotatorToTarget = UKismetMathLibrary::FindLookAtRotation(SkeletalMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
        RotatorToTarget.Yaw -= 90.0f;
        FocusOnTarget(RotatorToTarget);
        CurrentTarget->AdjustAttackerLocation(this);
        UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);
        BattleController->EnemyUseSkill();
    }
    else
    {
        AdjustFrontCamera();
    }
}

void AEnemyUnitBase::AdjustFrontCamera()
{
    SwitchToFrontCamera(false);
    UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
    if(CurrentTarget.Get() != this)
    {
        FRotator RotatorToTarget = UKismetMathLibrary::FindLookAtRotation(SkeletalMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
        RotatorToTarget.Yaw -= 90.0f;
        FocusOnTarget(RotatorToTarget);
        UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);
    }
    SwitchToFrontCamera(false);
    UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);
    BattleController->EnemyUseSkill();
}

AUnitBase *AEnemyUnitBase::GetTargetUnit(bool bIsPlayer)
{
    ABattleBase *CurrentBattle = BattleController->GetCurrentBattle();
    AUnitBase *Target;
    int Index;
    if(bIsPlayer)
    {
        Index = UKismetMathLibrary::RandomIntegerInRange(0, CurrentBattle->GetPlayerUnits().Num() - 1);
        Target = Cast<AUnitBase>(CurrentBattle->GetPlayerUnits()[Index]);
    }
    else
    {
        Index = UKismetMathLibrary::RandomIntegerInRange(0, CurrentBattle->GetEnemyUnits().Num() - 1);
        Target = Cast<AUnitBase>(CurrentBattle->GetEnemyUnits()[Index]);
    }
    return Target;
}   

TSubclassOf<ABattleSkillBase> AEnemyUnitBase::GetRandomSkill()
{
    float TotalWeight = 0.0f;
    for(const auto& Pair : Skills)
    {
        TotalWeight += Pair.Value;
    }
    float RandomWeight = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);
    float CurrentWeight = 0.0f;
    TSubclassOf<ABattleSkillBase> Result;
    for(const auto& Pair : Skills)
    {
        float MaxSkillWeight = CurrentWeight + Pair.Value;
        if(RandomWeight <= MaxSkillWeight)
        {
            Result = Pair.Key;
            break; 
        }
        CurrentWeight = MaxSkillWeight;
    }
    return Result;
}