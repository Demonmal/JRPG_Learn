// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionsUI.h"
#include "ActionButton.h"

void UActionsUI::Init()
{
    AllActionButtons = {AttackButton, FleeButton, ItemButton, SkillButton};
    for (auto Button : AllActionButtons)
    {
        Button->OnActionButtonClicked.AddUObject(this, &UActionsUI::OnActionButtonClicked);
    }
}

void UActionsUI::ShowActionsUI()
{
    PlayAnimation(ShowAnimation);
    SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UActionsUI::HideActionsUI()
{
    StopAnimation(ShowAnimation);
    SetRenderOpacity(0.0f);
    SetVisibility(ESlateVisibility::Hidden);
}

void UActionsUI::OnActionButtonClicked(EActionType Type)
{
    if(OnActionPressed.IsBound())
    {
        OnActionPressed.Broadcast(Type);
    }
}