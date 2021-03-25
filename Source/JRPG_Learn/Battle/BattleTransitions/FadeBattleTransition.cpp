// Fill out your copyright notice in the Description page of Project Settings.

#include "FadeBattleTransition.h"
#include "Components/SceneCaptureComponent2D.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../UI/FadeTransitionUI.h"
#include "TimerManager.h"

AFadeBattleTransition::AFadeBattleTransition()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->bCaptureEveryFrame = false;
    SceneCaptureComponent->bCaptureOnMovement = false;
}

void AFadeBattleTransition::StartTransition()
{
    SceneCaptureComponent->SetWorldTransform(PlayerController->PlayerCameraManager->GetActorTransform());
    SceneCaptureComponent->CaptureScene();
    FadeTransitionUI = CreateWidget<UFadeTransitionUI>(GetWorld(), FadeTransitionUIClass);
    FadeTransitionUI->AddToViewport();
    FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    FTimerManager &WorldTimerManager = GetWorld()->GetTimerManager();
    TimerCallback.BindLambda([&]() {
        FadeTransitionUI->RemoveFromParent();
        PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 1.0f, 0.2f, FLinearColor::Black, false, true);
        TimerCallback.Unbind();
        TimerCallback.BindLambda([&]() {
            if (OnTransitionEnded.IsBound())
            {
                OnTransitionEnded.Broadcast();
                PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 0.5f, FLinearColor::Black, false, true);
            }
        });
        WorldTimerManager.SetTimer(TimerHandle, TimerCallback, 0.2f, false);
    });
    WorldTimerManager.SetTimer(TimerHandle, TimerCallback, 0.5f, false);
}