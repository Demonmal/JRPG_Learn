// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryDialogue.h"
#include "Components/Overlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../ActionButton.h"

UVictoryDialogue::UVictoryDialogue() : UUIBase::UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void UVictoryDialogue::OnConfirm()
{
    PlayAnimation(VictoryAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    SetUIInput(true);
    SetInputActionBlocking(false);
    if(OnContinueClicked.IsBound())
    {
        OnContinueClicked.Broadcast();
    }
    PlayConfirmSound();
    FLatentActionInfo LatentActionInfo;
    UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
    RemoveFromParent();
}

void UVictoryDialogue::ShowItemsObtained()
{
    if(UsableItemDrops.Num() > 0 || MiscItemDrops.Num() > 0 || EquipmentDrops.Num() > 0)
    {
        DropsOverlay->SetVisibility(ESlateVisibility::Visible);
        ShowUsableItemDrops();
        ShowMiscItemDrops();
        ShowEquipmentDrops();
    }
}