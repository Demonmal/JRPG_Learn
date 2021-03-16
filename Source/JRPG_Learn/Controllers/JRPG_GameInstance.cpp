// Fill out your copyright notice in the Description page of Project Settings.


#include "JRPG_GameInstance.h"
#include "AudioPlayerController.h"
#include "Engine/World.h"

void UJRPG_GameInstance::LoadGame()
{

}

void UJRPG_GameInstance::AddInteractions(class AInteractionDetector* InteractionDetector)
{

}

AAudioPlayerController *UJRPG_GameInstance::AudioPlayerController(UWorld* World)
{
    return IsValid(AudioPlayerControllerInstance) ? 
           AudioPlayerControllerInstance : 
           AudioPlayerControllerInstance = World->SpawnActor<AAudioPlayerController>(AAudioPlayerController::StaticClass());
}