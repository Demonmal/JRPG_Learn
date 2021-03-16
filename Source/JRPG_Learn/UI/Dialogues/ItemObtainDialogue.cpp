// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemObtainDialogue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "ItemButton.h"
#include "../../Units/PlayerUnits/PlayerUnitBase.h"

UItemObtainDialogue::UItemObtainDialogue() : UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void UItemObtainDialogue::CallOnConfirmed()
{
    if (OnConfirmed.IsBound())
    {
        OnConfirmed.Broadcast();
    }
}

void UItemObtainDialogue::ShowObtainedPlayerUnit()
{
    if (UKismetSystemLibrary::IsValidClass(PlayerUnit))
    {
        APlayerUnitBase *UnitDefault = Cast<APlayerUnitBase>(PlayerUnit->GetDefaultObject());
        PlayerUnitRewardOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
        UnitName->SetText(UnitDefault->GetTitle());
        UnitImage->SetBrushFromTexture(UnitDefault->GetBattleImage());
    }
}

void UItemObtainDialogue::ShowObtainedUsableItems()
{
    for (const auto &Pair : UsableItems)
    {
        UItemButton* ItemButton = CreateWidget<UItemButton>(GetWorld(), ItemButtonClass);
        ItemButton->Item = Pair.Key;
        ItemButton->Amount = Pair.Value;
        ItemList->AddChildToVerticalBox(ItemButton);
    }
}

void UItemObtainDialogue::ShowObtainedMiscItems()
{
    for (const auto &Pair : MiscItems)
    {
        UItemButton* ItemButton = CreateWidget<UItemButton>(GetWorld(), ItemButtonClass);
        ItemButton->Item = Pair.Key;
        ItemButton->Amount = Pair.Value;
        ItemList->AddChildToVerticalBox(ItemButton);
    }
}

void UItemObtainDialogue::ShowObtainedEquipment()
{
    for (const auto &Pair : Equipment)
    {
        UItemButton* ItemButton = CreateWidget<UItemButton>(GetWorld(), ItemButtonClass);
        ItemButton->Item = Pair.Key;
        ItemButton->Amount = Pair.Value;
        ItemList->AddChildToVerticalBox(ItemButton);
    }
}

void UItemObtainDialogue::ShowObtainedGold()
{
    if (Gold > 0)
    {
        GoldRewardOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
        FString String = FString::Printf(TEXT("+%i"), Gold);
        GoldRewardAmount->SetText(FText::FromString(String));
    }
}