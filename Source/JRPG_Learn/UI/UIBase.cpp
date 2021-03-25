// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBase.h"
#include "../Controllers/AudioPlayerController.h"
#include "../Controllers/JRPG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UUIBase::NativeConstruct()
{
	AudioPlayerController = Cast<UJRPG_GameInstance>(GetWorld()->GetGameInstance())->AudioPlayerController(GetWorld());
	Super::NativeConstruct();
}

void UUIBase::FocusOnThisUI()
{
    SetUIInput(false);
    bIsFocusable = false;
    bStopAction = true;
    SetInputActionPriority(0);
    StopListeningForAllInputActions();
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(true);
    InputMode.SetWidgetToFocus(TakeWidget());
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
    ListenInputActions();
    UE_LOG(LogTemp, Log, TEXT("FocusOnThisUI"))
}

void UUIBase::ListenInputActions()
{
    ListenMenuUp.BindUFunction(this, "ListenMenuUpLogic");
    ListenMenuDown.BindUFunction(this, "ListenMenuDownLogic");
    ListenMenuRight.BindUFunction(this, "ListenMenuRightLogic");
    ListenMenuLeft.BindUFunction(this, "ListenMenuLeftLogic");
    ListenMenuConfirm.BindUFunction(this, "ListenMenuConfirmLogic");
    ListenMenuCancel.BindUFunction(this, "ListenMenuCancelLogic");
    ListenMenuDetails.BindUFunction(this, "ListenMenuDetailsLogic");
    ListenForInputAction("MenuUp", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuUp);
    ListenForInputAction("MenuDown", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuDown);
    ListenForInputAction("MenuRight", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuRight);
    ListenForInputAction("MenuLeft", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuLeft);
    ListenForInputAction("MenuConfirm", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuConfirm);
    ListenForInputAction("MenuCancel", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuCancel);
    ListenForInputAction("MenuDetails", EInputEvent::IE_Pressed, false, UUIBase::ListenMenuDetails);
}

void UUIBase::ListenMenuUpLogic()
{
    if(!bIsUIInputBlocked)
    {
        MenuUp();
    }
}

void UUIBase::ListenMenuDownLogic()
{
    if(!bIsUIInputBlocked)
    {
        MenuDown();
    }
}

void UUIBase::ListenMenuRightLogic()
{
    if(!bIsUIInputBlocked)
    {
        MenuRight();
    }
}

void UUIBase::ListenMenuLeftLogic()
{
    if(!bIsUIInputBlocked)
    {
        MenuLeft();
    }
}

void UUIBase::ListenMenuConfirmLogic()
{
    UE_LOG(LogTemp, Log, TEXT("MenuConfirm Pressed %i"), bIsUIInputBlocked ? 1 : 0)
    if(!bIsUIInputBlocked)
    {
        MenuConfirm();
    }
}

void UUIBase::ListenMenuCancelLogic()
{
    UE_LOG(LogTemp, Log, TEXT("MenuCancel Pressed %i"), bIsUIInputBlocked ? 1 : 0)
    if(!bIsUIInputBlocked)
    {
        MenuCancel();
    }
}

void UUIBase::ListenMenuDetailsLogic()
{
    if(!bIsUIInputBlocked)
    {
        MenuDetails();
    }
}

void UUIBase::PlayConfirmSound()
{
    AudioPlayerController->PlaySoundByTag("Confirm");
}

void UUIBase::SetUIInput(bool bIsBlocked)
{
    
}