// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButton.h"
#include "../../Controllers/AudioPlayerController.h"
#include "../../Controllers/JRPG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../Items/ItemBase.h"

void UItemButton::NativeConstruct()
{
	AudioPlayerController = Cast<UJRPG_GameInstance>(GetWorld()->GetGameInstance())->AudioPlayerController(GetWorld());
	Super::NativeConstruct();
}

void UItemButton::SetItemTextAndTexture()
{
    AItemBase* ItemDefault = Cast<AItemBase>(Item->GetDefaultObject());
    ItemName->SetText(ItemDefault->GetName());
    ItemImage->SetBrushFromTexture(ItemDefault->GetIcon());
}

void UItemButton::CallOnItemButtonClicked()
{
    if(OnItemButtonClicked.IsBound())
    {
        OnItemButtonClicked.Broadcast(Item, this);
    }
}

void UItemButton::PlayMenuNavigationSound()
{
    AudioPlayerController->PlaySoundByTag("MenuNavigation");
}

void UItemButton::SetAmount()
{
    FString String = FString::Printf(TEXT("x %i"), Amount);
    AmountText->SetText(FText::FromString(String));
}

void UItemButton::SetEmpty()
{
    ItemImage->SetVisibility(ESlateVisibility::Collapsed);
    AmountText->SetVisibility(ESlateVisibility::Collapsed);
    FString EmptyString = "";
    ItemName->SetText(FText::FromString(EmptyString));
}