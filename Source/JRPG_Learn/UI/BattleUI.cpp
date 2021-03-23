// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUI.h"
#include "../Units/UnitBase.h"
#include "../Battle/BattleController.h"
#include "../Battle/BattleBase.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Items/ItemBase.h"
#include "../Skills/BattleSkillBase.h"
#include "TurnOrderList.h"
#include "UnitBattleDetails.h"
#include "PlayerUnitsListUI.h"
#include "Dialogues/ItemUseDialogue.h"
#include "Dialogues/SkillUseDialogue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBattleUI::ShowBattleUI()
{
    BP_PlayerUnitsList->SetPlayerUnitUIList(CurrentBattle->GetPlayerUnits(), BattleController.Get());
    PlayAnimation(ShowPlayerUnitListUIAnim);
    PlayAnimation(TargetImageAnim);
    ShowBossUI();
}

void UBattleUI::HideBattleUI()
{
    PlayAnimation(ShowPlayerUnitListUIAnim, .0f, 1, EUMGSequencePlayMode::Reverse);
    PlayAnimation(ShowTurnOrderUI, .0f, 1, EUMGSequencePlayMode::Reverse);
    FLatentActionInfo LatentActionInfo;
    UKismetSystemLibrary::Delay(GetWorld(), .1f, LatentActionInfo);
    RemoveFromParent();
}

void UBattleUI::ShowBossUI()
{
}

void UBattleUI::ShowUsableItemInventory()
{
    BP_ItemUseDialogue->ShowUsableItems(CurrentBattle->GetPlayerController()->GetUsableItems());
    UnbindAllItemEvents();
    BP_ItemUseDialogue->ShowUsableItemInventory();
    BP_ItemUseDialogue->OnItemSelected.AddUObject(this, &UBattleUI::OnItemSelectedHandler);
    BP_ItemUseDialogue->OnCanceled.AddUObject(this, &UBattleUI::OnItemCancelledHandler);
}

void UBattleUI::OnItemSelectedHandler(TSubclassOf<AItemBase> SelectedItemClass)
{
    UnbindAllItemEvents();
    if (OnItemSelected.IsBound())
    {
        OnItemSelected.Broadcast(SelectedItemClass);
    }
    HideUsableItemInventory();
    BP_ItemUseDialogue->OnItemSelected.AddUObject(this, &UBattleUI::OnItemUsedHandler);
    BP_ItemUseDialogue->OnCanceled.AddUObject(this, &UBattleUI::OnItemUseCancelledHandler);
}

void UBattleUI::OnItemUsedHandler(TSubclassOf<AItemBase> SelectedItemClass)
{
    UnbindAllItemEvents();
    FInputModeGameOnly InputModeGameOnly;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeGameOnly);
    if (OnItemUsed.IsBound())
    {
        OnItemUsed.Broadcast();
    }
}

void UBattleUI::OnItemUseCancelledHandler()
{
    UnbindAllItemEvents();
    if (OnItemUseCancelled.IsBound())
    {
        OnItemUseCancelled.Broadcast();
    }
}

void UBattleUI::OnItemCancelledHandler()
{
    UnbindAllItemEvents();
    if (OnItemInventoryClosed.IsBound())
    {
        OnItemInventoryClosed.Broadcast();
    }
}

void UBattleUI::HideUsableItemInventory()
{
    BP_ItemUseDialogue->PlayItemInventoryAnim(EUMGSequencePlayMode::Reverse);
    FInputModeGameOnly InputModeGameOnly;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeGameOnly);
}

void UBattleUI::HideItemsActionButtons()
{
    BP_ItemUseDialogue->PlayItemActionButtonsAnim(EUMGSequencePlayMode::Reverse);
}

void UBattleUI::UnbindAllItemEvents()
{
    BP_ItemUseDialogue->OnItemSelected.RemoveAll(this);
    BP_ItemUseDialogue->OnCanceled.RemoveAll(this);
}

void UBattleUI::UseItem()
{
    HideItemsActionButtons();
}

void UBattleUI::ShowSkills(APlayerUnitBase *PlayerUnit)
{
    UnbindAllSkillEvents();
    BP_SkillUseDialogue->SetSkills(PlayerUnit->GetAvailableBattleSkills());
    BP_SkillUseDialogue->ShowSkills();
    BP_SkillUseDialogue->OnSkillSelected.AddUObject(this, &UBattleUI::OnSkillSelectedHandler);
    BP_SkillUseDialogue->OnCanceled.AddUObject(this, &UBattleUI::OnSkillCancelledHandler);
}

void UBattleUI::OnSkillSelectedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill)
{
    if (OnSkillSelected.IsBound())
    {
        OnSkillSelected.Broadcast(SelectedSkill);
    }
}

void UBattleUI::OnSkillCancelledHandler()
{
    UnbindAllSkillEvents();
    if (OnSkillUIClosed.IsBound())
    {
        OnSkillUIClosed.Broadcast();
    }
}

void UBattleUI::OnUnitHasEnoughMP()
{
    UnbindAllSkillEvents();
    HideSkills();
    BP_SkillUseDialogue->OnSkillSelected.AddUObject(this, &UBattleUI::OnSkillUsedHandler);
    BP_SkillUseDialogue->OnCanceled.AddUObject(this, &UBattleUI::OnSkillUseCancelledHandler);
}

void UBattleUI::OnSkillUsedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill)
{
    UnbindAllSkillEvents();
    FInputModeGameOnly InputModeGameOnly;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeGameOnly);
    if (OnSkillUsed.IsBound())
    {
        OnSkillUsed.Broadcast();
    }
}

void UBattleUI::OnSkillUseCancelledHandler()
{
    UnbindAllSkillEvents();
    if (OnSkillUseCancelled.IsBound())
    {
        OnSkillUseCancelled.Broadcast();
    }
}

void UBattleUI::UnbindAllSkillEvents()
{
    BP_SkillUseDialogue->OnSkillSelected.RemoveAll(this);
    BP_SkillUseDialogue->OnCanceled.RemoveAll(this);
}

void UBattleUI::HideSkills()
{
    BP_SkillUseDialogue->PlayShowSkillsAnim(EUMGSequencePlayMode::Reverse);
    FInputModeGameOnly InputModeGameOnly;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeGameOnly);
}

void UBattleUI::HideSkillActionButtons()
{
    BP_SkillUseDialogue->PlaySkillActionButtonsAnim(EUMGSequencePlayMode::Reverse);
}

void UBattleUI::ShowUnitBattleDetails()
{
    if (!bIsUnitBattleDetailsShown)
    {
        PlayAnimation(ShowUnitBattleDetailsAnim);
        bIsUnitBattleDetailsShown = true;
    }
}

void UBattleUI::HideUnitBattleDetails()
{
    if (bIsUnitBattleDetailsShown)
    {
        PlayAnimation(ShowUnitBattleDetailsAnim, .0f, 1, EUMGSequencePlayMode::Reverse);
        bIsUnitBattleDetailsShown = false;
    }
}

void UBattleUI::UpdateAttackingUnitBattleDetails(AUnitBase *Unit)
{
    AttackingUnitBattleDetails->UpdateUnitDetails(Unit);
}

void UBattleUI::UpdateTargetUnitBattleDetails(AUnitBase *Unit)
{
    TargetUnitBattleDetails->UpdateUnitDetails(Unit);
}

void UBattleUI::InitTurnOrderUI()
{
    if (bIsTurnOrderInited)
    {
        BP_TurnOrderList->InitTurnOrderList(BattleController.Get());
        PlayAnimation(ShowTurnOrderUI);
    }
}

void UBattleUI::UpdateTurnOrderList(AUnitBase *Unit)
{
    if (BattleController->GetShowTurnOrderUI())
    {
        InitTurnOrderUI();
        BP_TurnOrderList->UpdateTurnOderList(Unit);
    }
}