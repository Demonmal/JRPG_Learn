// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionButton.h"
#include "../Controllers/JRPG_FunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Controllers/JRPG_GameInstance.h"
#include "../Controllers/AudioPlayerController.h"

bool UActionButton::Initialize()
{
    GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    return Super::Initialize();
}

void UActionButton::CallOnActionButtonClicked(EActionType Type)
{
    if(OnActionButtonClicked.IsBound())
    {
        OnActionButtonClicked.Broadcast(ActionType);
    }
    GameInstance->AudioPlayerController(GetWorld())->PlaySoundByTag("Confirm");
}