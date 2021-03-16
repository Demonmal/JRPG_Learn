// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnitBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../../UI/ActionsUI.h"
#include "../../Skills/BattleSkillBase.h"
#include "../../Skills/ExploreSkillBase.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../Battle/BattleController.h"
#include "../../Controllers/JRPG_FunctionLibrary.h"

APlayerUnitBase::APlayerUnitBase() : AUnitBase::AUnitBase()
{
    ActionButtonsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionButtonsWidget"));
    ActionButtonsWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ActionButtonsWidget->SetDrawAtDesiredSize(true);
    ActionButtonsWidget->bEditableWhenInherited = 1;
    ActionButtonsWidget->SetupAttachment(RootComponent);
}

void APlayerUnitBase::InitUnit(ABattleController *Controller)
{
    AUnitBase::InitUnit(Controller);
    SetPlayerHPMP();
    SetBattleSkills();
    ActionsUI = Cast<UActionsUI>(ActionButtonsWidget->GetUserWidgetObject());
    ActionsUI->Init();
    BackCameraSpringArm->SetRelativeRotation(GetActorRotation());
}

void APlayerUnitBase::SetPlayerHPMP()
{
    FPlayerUnitData Data;
    if(BattleController->GetPlayerController()->TryGetUnitDataByPlayer(this->StaticClass(), Data))
    {
        SetHP(Data.CurrentHP);
        SetMP(Data.CurrentMP);
    }
}

void APlayerUnitBase::SetBattleSkills()
{
    for (const auto &pair : BattleSkills)
    {
        if (pair.Value <= Level)
        {
            AvailableBattleSkills.Add(pair.Key);
        }
    }
    FPlayerUnitData Data;
    if (BattleController->GetPlayerController()->TryGetUnitDataByPlayer(this->StaticClass(), Data))
    {
        TArray<TSubclassOf<ABattleSkillBase>> EquipmentBattleSkills;
        TArray<TSubclassOf<AExploreSkillBase>> EquipmentExploreSkills;
        UJRPG_FunctionLibrary::GetEquipmentSkills(Data, EquipmentBattleSkills, EquipmentExploreSkills);
        AvailableBattleSkills.Append(EquipmentBattleSkills);
    }
}

void APlayerUnitBase::SetUnitStats()
{
    FPlayerUnitData Data;
    if (!BattleController->GetPlayerController()->TryGetUnitDataByPlayer(this->StaticClass(), Data))
        return;
    FUnitStats Stats = UJRPG_FunctionLibrary::GetUnitStats(Level, MaxLevel, InitialStats, FirstLevelStats, LastLevelStats);
    FUnitStats EquipmentStats = UJRPG_FunctionLibrary::GetEquipmentStats(Data);
    FUnitStats ResultStats = Stats + EquipmentStats;
    UJRPG_FunctionLibrary::MakeAbsoluteStats(ResultStats);
    MaxHP = ResultStats.MaxHP;
    MaxMP = ResultStats.MaxMP;
    MinAttack = ResultStats.MinAttack;
    MaxAttack = ResultStats.MaxAttack;
    Defense = ResultStats.Defense;
    Speed = ResultStats.Speed;
    Hit = ResultStats.Hit;
    MinMagicalAttack = ResultStats.MinMagicalAttack;
    MaxMagicalAttack = ResultStats.MaxMagicalAttack;
    MagicDefense = ResultStats.MagicDefense;
    ActionTime = ResultStats.ActionTime;
}

void APlayerUnitBase::AdjustBackCamera(AUnitBase *TargetUnit, bool bIsInstant)
{
    if (IsValid(BackCameraArrow))
    {
        BackCameraArrow->DestroyComponent();
    }
    BackCameraArrow = NewObject<UArrowComponent>(this, UArrowComponent::StaticClass(), TEXT("BackCameraArrow"), RF_NoFlags);
    BackCameraArrow->RegisterComponent();

    FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetUnit->GetCameraLookLocation()->GetComponentLocation());
    FRotator ArrowRotator{LookAtRotator.Pitch, LookAtRotator.Yaw, BackCameraActor->GetComponentRotation().Roll};
    BackCameraArrow->SetWorldRotation(ArrowRotator);

    FVector Offset = BackCameraArrow->GetForwardVector() * BackCameraSpringArm->TargetArmLength * -1;
    BackCameraArrow->SetWorldLocation(Offset + BackCameraArrow->GetComponentLocation());

    LookAtRotator = UKismetMathLibrary::FindLookAtRotation(BackCameraArrow->GetComponentLocation(), TargetUnit->GetCameraLookLocation()->GetComponentLocation());
    BackCameraArrow->SetWorldRotation(FRotator(LookAtRotator.Pitch, LookAtRotator.Yaw, 0));

    CameraStartTransform = BackCameraArrow->GetComponentTransform();

    auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}