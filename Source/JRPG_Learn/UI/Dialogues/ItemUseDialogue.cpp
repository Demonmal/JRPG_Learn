// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUseDialogue.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "../ActionButton.h"
#include "ItemButton.h"
#include "../../Items/ItemBase.h"
#include "../../Items/UsableItems/UsableItemBase.h"

UItemUseDialogue::UItemUseDialogue() : UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void UItemUseDialogue::PlayItemInventoryAnim(EUMGSequencePlayMode::Type PlayMode)
{
    PlayAnimation(ShowItemInventory, .0f, 1, PlayMode);
}

void UItemUseDialogue::PlayItemActionButtonsAnim(EUMGSequencePlayMode::Type PlayMode)
{
    PlayAnimation(ShowItemActionButtons, .0f, 1, PlayMode);
}

void UItemUseDialogue::ShowUsableItemInventory()
{
    PlayItemInventoryAnim(EUMGSequencePlayMode::Type::Forward);
    PlayItemActionButtonsAnim(EUMGSequencePlayMode::Type::Forward);
    FocusOnThisUI();
}

void UItemUseDialogue::OnItemButtonClickedHandler(TSubclassOf<AItemBase> ItemClass, UItemButton *ItemButton)
{
    SelectedItemIndex = ItemButtons.Find(ItemButton);
    CheckIfSelectedItemAndHighlightNewOne();
}

void UItemUseDialogue::CheckIfSelectedItemAndHighlightNewOne()
{
    if (IsValid(HighlightedItem))
    {
        HighlightedItem->Highlight(false);
    }
    HighlightSelectedItem();
}

void UItemUseDialogue::HighlightSelectedItem()
{
    HighlightedItem = ItemButtons[SelectedItemIndex];
    HighlightedItem->Highlight(true);
    ItemDescriptionOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
    AItemBase *ItemDefault = Cast<AItemBase>(HighlightedItem->Item->GetDefaultObject());
    ItemDescriptionText->SetText(ItemDefault->GetShortDescription());
    EmptyText->SetVisibility(ESlateVisibility::Hidden);
}

void UItemUseDialogue::ShowUsableItems(TMap<TSubclassOf<AUsableItemBase>, int> UsableItems)
{
    EmptyText->SetVisibility(ESlateVisibility::HitTestInvisible);
    HighlightedItem = nullptr;
    ItemDescriptionOverlay->SetVisibility(ESlateVisibility::Collapsed);
    ItemList->ClearChildren();
    ItemButtons.Empty();
    for (const auto &Item : UsableItems)
    {
        UItemButton *Button = CreateWidget<UItemButton>(GetWorld(), ItemButtonClass);
        ItemList->AddChild(Button);
        ItemButtons.Add(Button);
        Button->OnItemButtonClicked.AddUObject(this, &UItemUseDialogue::OnItemButtonClickedHandler);
    }
    SelectFirstItem();
    ConfirmButton->OnActionButtonClicked.AddUObject(this, &UItemUseDialogue::Confirm);
    CancelButton->OnActionButtonClicked.AddUObject(this, &UItemUseDialogue::Cancel);
}

void UItemUseDialogue::CallOnItemSelected(TSubclassOf<AItemBase> SelectedItem)
{
    if (OnItemSelected.IsBound())
    {
        OnItemSelected.Broadcast(SelectedItem);
    }
}

void UItemUseDialogue::CallOnCanceled()
{
    if (OnCanceled.IsBound())
    {
        OnCanceled.Broadcast();
    }
}