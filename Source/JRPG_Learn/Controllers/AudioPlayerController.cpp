// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioPlayerController.h"
#include "Sound/SoundCue.h"
#include "JRPG_GameInstance.h"
#include "../Data/DataTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"

void AAudioPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SoundsTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/JRPG_Learn/Data/Tables/T_Sounds"));
}

void AAudioPlayerController::PlaySoundByTag(FName Tag)
{
	UE_LOG(LogTemp, Log, TEXT("AAudioPlayerController::PlaySoundByTag %s"), *Tag.ToString())
	FSound* SoundContainer = SoundsTable->FindRow<FSound>(Tag, "");
	if(SoundContainer)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundContainer->Sound);
	}
}