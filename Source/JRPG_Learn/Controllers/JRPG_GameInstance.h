// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "JRPG_GameInstance.generated.h"

class UJRPG_SaveGame;
class AAudioPlayerController;
class ABattleController;

UCLASS()
class JRPG_LEARN_API UJRPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:

	void LoadGame();
	void AddInteractions(class AInteractionDetector*);

	UFUNCTION()
	AAudioPlayerController *AudioPlayerController(class UWorld* World);
	UFUNCTION()
	void SetBattleControllerInstance(ABattleController *Controller);

	FORCEINLINE ABattleController *GetBattleController() const {return BattleControllerInstance.Get();}

	FORCEINLINE FString GetSlotName() const {return SlotName;}
	FORCEINLINE UJRPG_SaveGame* GetSaveGameObject() const {return SaveGameObject;}
	FORCEINLINE bool GetShouldLoadGame() const {return bShouldLoadGame;}
	FORCEINLINE bool GetShouldLoadTransform() const {return bShouldLoadTransform;}
	FORCEINLINE bool GetShouldLoadEnemyPawns() const {return bShouldLoadEnemyPawns;}

	FStreamableManager StreamableManager;

	private:

	UPROPERTY()
	AAudioPlayerController *AudioPlayerControllerInstance;
	UPROPERTY()
	TWeakObjectPtr<ABattleController> BattleControllerInstance;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAudioPlayerController> AudioPlayerControllerClass;

	UPROPERTY(EditAnywhere)
	FString SlotName;
	UPROPERTY(VisibleAnywhere)
	UJRPG_SaveGame* SaveGameObject;
	UPROPERTY(VisibleAnywhere)
	bool bShouldLoadGame;
	UPROPERTY(VisibleAnywhere)
	bool bShouldLoadTransform;
	UPROPERTY(VisibleAnywhere)
	bool bShouldLoadEnemyPawns;
};
