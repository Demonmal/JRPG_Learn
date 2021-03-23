// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnitBase.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
#include "../../Battle/BattleBase.h"
#include "../../Controllers/JRPG_FunctionLibrary.h"
#include "../../Controllers/JRPG_GameInstance.h"
#include "../../Controllers/AudioPlayerController.h"

APlayerUnitBase::APlayerUnitBase() : AUnitBase::AUnitBase()
{
    ActionButtonsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionButtonsWidget"));
    ActionButtonsWidget->SetWidgetSpace(EWidgetSpace::Screen);
    ActionButtonsWidget->SetDrawAtDesiredSize(true);
    ActionButtonsWidget->bEditableWhenInherited = 1;
    ActionButtonsWidget->SetupAttachment(RootComponent);
}

void APlayerUnitBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	BackCameraAdjustTimeline.TickTimeline(DeltaSeconds);
}

void APlayerUnitBase::InitUnit(ABattleController *Controller)
{
    AUnitBase::InitUnit(Controller);
    UJRPG_GameInstance *GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    AudioController = GameInstance->AudioPlayerController(GetWorld());
    SetPlayerHPMP();
    SetBattleSkills();
    ActionsUI = Cast<UActionsUI>(ActionButtonsWidget->GetUserWidgetObject());
    ActionsUI->Init();
    ActionsUI->OnActionPressed.AddUObject(this, &APlayerUnitBase::OnActionPressedHandler);
    BackCameraSpringArm->SetRelativeRotation(GetActorRotation());
}

void APlayerUnitBase::OnActionPressedHandler(EActionType ActionType)
{
    switch (ActionType)
    {
    case EActionType::Attack:
        RemoveUI();
        BattleController->Attack(CurrentTarget.Get());
        AudioController->PlaySoundByTag("Confirm");
        break;
    case EActionType::Skill:
        RemoveUI();
        BattleController->ShowSkills(this);
        AudioController->PlaySoundByTag("Confirm");
        break;
    case EActionType::Item:
        RemoveUI();
        BattleController->ShowItemInventory();
        AudioController->PlaySoundByTag("Confirm");
        break;
    case EActionType::Flee:
        RemoveUI();
        BattleController->Flee(this);
        break;
    }
}

void APlayerUnitBase::SetPlayerHPMP()
{
    FPlayerUnitData Data;
    if (BattleController->GetPlayerController()->TryGetUnitDataByPlayer(this->StaticClass(), Data))
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

    if(PlayerController->GetViewTarget() == BackCameraActor->GetChildActor())
    {
        if (IsValid(BackCameraAdjustTimelineCurve))
	    {
		    FOnTimelineFloatStatic BackCameraAdjustTimelineUpdate;
            FOnTimelineEventStatic BackCameraAdjustTimelineFinish;
		    BackCameraAdjustTimelineUpdate.BindUObject(this, &APlayerUnitBase::BackCameraAdjustTimelineUpdate);
            BackCameraAdjustTimelineFinish.BindUObject(this, &APlayerUnitBase::BackCameraAdjustTimelineFinished);
		    BackCameraAdjustTimeline.AddInterpFloat(BackCameraAdjustTimelineCurve, BackCameraAdjustTimelineUpdate);
            BackCameraAdjustTimeline.SetTimelineFinishedFunc(BackCameraAdjustTimelineFinish);
		    BackCameraAdjustTimeline.Play();
	    }
    }
    else
    {
        BackCameraActor->SetWorldTransform(BackCameraArrow->GetComponentTransform());
        PlayerController->SetViewTargetWithBlend(BackCameraActor->GetChildActor(), bIsInstant ? 0.0f : 0.5f);
        if(IsValid(BackCameraArrow))
        {
            BackCameraArrow->DestroyComponent();
        }
    }
}

void APlayerUnitBase::BackCameraAdjustTimelineUpdate(const float Alpha)
{
    FTransform BackCameraAttowTransform = BackCameraArrow->GetComponentTransform();
    FTransform NewTransform;
    NewTransform.SetLocation(UKismetMathLibrary::VLerp(CameraStartTransform.GetLocation(), BackCameraAttowTransform.GetLocation(), Alpha));
    NewTransform.SetRotation(FQuat::Slerp(CameraStartTransform.GetRotation(), BackCameraAttowTransform.GetRotation(), Alpha));
    BackCameraActor->SetWorldTransform(NewTransform);
}

void APlayerUnitBase::BackCameraAdjustTimelineFinished()
{
    if(IsValid(BackCameraArrow))
    {
        BackCameraArrow->DestroyComponent();
    }
}

void APlayerUnitBase::StartTurn()
{
    CurrentActionType = EActionType::Attack;
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
        EnableInput(PlayerController.Get());
        CurrentTargetIndex = 0;
        ActionsUI->ShowActionsUI();
        SetEnemyTargetAndAdjustCamera();
    }
}

FTransform APlayerUnitBase::GetProjectileSpawnTransform()
{
    return SkeletalMesh->GetSocketTransform("ProjectileSocket");
}

void APlayerUnitBase::ChangeEnemyTarget(bool bIsRight)
{
    CurrentTarget->SetTargetIconVisibility(false);
    int Index = CurrentTargetIndex;
    int LastEnemyUnitsIndex = BattleController->GetCurrentBattle()->GetEnemyUnits().Num() - 1;
    if(bIsRight)
    {
        Index = Index - 1 < 0 ? LastEnemyUnitsIndex : Index - 1;
    }
    else
    {
        Index = Index + 1 > LastEnemyUnitsIndex ? 0 : Index + 1;
    }
    CurrentTargetIndex = Index;
    AudioController->PlaySoundByTag("MenuNavigation");
    SetEnemyTargetAndAdjustCamera();
}

void APlayerUnitBase::ChangePlayerTarget(bool bIsRight)
{
    CurrentTarget->SetTargetIconVisibility(false);
    int Index = CurrentTargetIndex;
    int LastEnemyUnitsIndex = BattleController->GetCurrentBattle()->GetPlayerUnits().Num() - 1;
    if(bIsRight)
    {
        Index = Index - 1 < 0 ? LastEnemyUnitsIndex : Index - 1;
    }
    else
    {
        Index = Index + 1 > LastEnemyUnitsIndex ? 0 : Index + 1;
    }
    CurrentTargetIndex = Index;
    AudioController->PlaySoundByTag("MenuNavigation");
    SetPlayerTargetAndAdjustCamera();
}

void APlayerUnitBase::ChangeTargetForUseItem()
{
    CurrentActionType = EActionType::Item;
    EnableInput(PlayerController.Get());
    CurrentTargetIndex = BattleController->GetCurrentBattle()->GetPlayerUnits().Find(this);
    SetPlayerTargetAndAdjustCamera();
}

void APlayerUnitBase::ChangeCameraForSkill(TSubclassOf<ABattleSkillBase> Skill)
{
    CurrentSkill = Skill;
    CurrentActionType = EActionType::Skill;
    EnableInput(PlayerController.Get());
    ABattleSkillBase *SkillBase = Cast<ABattleSkillBase>(Skill->GetDefaultObject());
    if(SkillBase->GetSkillType() == ESkillType::Offensive)
    {
        SetEnemyTargetAndAdjustCamera();
    }
    else
    {
        CurrentTargetIndex = BattleController->GetCurrentBattle()->GetPlayerUnits().Find(this);
        SetPlayerTargetAndAdjustCamera();
    }
}

void APlayerUnitBase::SetEnemyTargetAndAdjustCamera()
{
    CurrentTarget = GetTargetUnit(false);
    CurrentTarget->SetTargetIconVisibility(true);
    CurrentTarget->AdjustAttackerLocation(this);
    FRotator RotatorToTarget = UKismetMathLibrary::FindLookAtRotation(SkeletalMesh->GetComponentLocation(), CurrentTarget->GetActorLocation());
    RotatorToTarget.Yaw -= 90.0f;
    FocusOnTarget(RotatorToTarget);
    BattleController->AdjustCamera(this, CurrentTarget.Get(), false);
    BattleController->UpdateUnitBattleDetails(Cast<AUnitBase>(this), CurrentTarget.Get());
}

void APlayerUnitBase::SetPlayerTargetAndAdjustCamera()
{
    CurrentTarget = GetTargetUnit(true);
    CurrentTarget->SetTargetIconVisibility(true);
    SwitchToFrontCamera(false);
    BattleController->UpdateUnitBattleDetails(Cast<AUnitBase>(this), CurrentTarget.Get());
}

AUnitBase *APlayerUnitBase::GetTargetUnit(bool bIsPlayer)
{
    ABattleBase *CurrentBattle = BattleController->GetCurrentBattle();
    AUnitBase *Target;
    if(bIsPlayer)
    {
        Target = Cast<AUnitBase>(CurrentBattle->GetPlayerUnits()[CurrentTargetIndex]);
    }
    else
    {
        Target = Cast<AUnitBase>(CurrentBattle->GetEnemyUnits()[CurrentTargetIndex]);
    }
    return Target;
}

void APlayerUnitBase::RemoveUI()
{
    CurrentTarget->SetTargetIconVisibility(false);
    DisableInput(PlayerController.Get());
    ActionsUI->HideActionsUI();
}

void APlayerUnitBase::OnDied()
{
    FLatentActionInfo LatentActionInfo;
    UKismetSystemLibrary::Delay(GetWorld(), .5f, LatentActionInfo);
    Destroy();
}