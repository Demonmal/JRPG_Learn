// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnitUI.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Battle/BattleController.h"
#include "../Units/UnitBase.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "Components/Image.h"
#include "HPBar.h"
#include "MPBar.h"
#include "ActionTimeBar.h"

void UPlayerUnitUI::Init(ABattleController *BattleController)
{
    UnitImage->SetBrushFromTexture(PlayerUnit->GetBattleImage());
    BP_HPBar->SetUnit(PlayerUnit.Get());
    BP_MPBar->SetUnit(PlayerUnit.Get());
    PlayerUnit->OnTurnStarted.AddUObject(this, &UPlayerUnitUI::OnTurnStartedHandler);
    PlayerUnit->OnTurnEnded.AddUObject(this, &UPlayerUnitUI::OnTurnEndedHandler);
    PlayerUnit->OnUnitDied.AddUObject(this, &UPlayerUnitUI::OnUnitDiedHandler);
    ETurnType TurnType = BattleController->GetTurnType();
    if(TurnType == ETurnType::ActionTime)
    {
        BP_ActionTimeBar->SetUnit(PlayerUnit.Get());
    }
    else
    {
        BP_ActionTimeBar->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UPlayerUnitUI::OnTurnStartedHandler()
{
    Background->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUnitUI::OnTurnEndedHandler()
{
    Background->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerUnitUI::OnUnitDiedHandler(AUnitBase* DiedUnit)
{
    FLatentActionInfo LatentActionInfo;
    UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
    RemoveFromParent();
}