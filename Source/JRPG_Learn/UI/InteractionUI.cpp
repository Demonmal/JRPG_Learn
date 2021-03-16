// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionUI.h"
#include "ActionButton.h"

void UInteractionUI::Init(FText InteractionText)
{
    InteractionButton->OnActionButtonClicked.AddLambda([&](EActionType Type)
    {
        if(OnInteracted.IsBound())
        {
            OnInteracted.Broadcast();
        }
    });
    InteractionButton->Action = InteractionText;
}

void UInteractionUI::ShowInteractionUI()
{
    PlayAnimation(ShowAnimation);
}

void UInteractionUI::HideInteractionUI()
{
    StopAnimation(ShowAnimation);
    SetRenderOpacity(.0f);
}