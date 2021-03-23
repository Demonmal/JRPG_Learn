// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/DataTypes.h"
#include "../Data/Enums.h"
#include "Components/TimelineComponent.h"
#include "EnemyExplorePawnBase.generated.h"

class AInteractionBattle;
class AInteractionDetector;
class AJRPG_PlayerController;
class AEnemyUnitBase;

UCLASS(Abstract)
class JRPG_LEARN_API AEnemyExplorePawnBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyExplorePawnBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartBattle();
	virtual void EnableAI();
	virtual void StopAI();
	virtual void DisableAI();

protected:

	virtual void BeginPlay() override;

	void OnInteractedHandler();
	void OnLoadedHandler(bool bShouldLoadEnemyPawns);
	void OnGameStateChangedHandler(EGameState GameState);
	void OnWaitFinished();

	UPROPERTY(VisibleDefaultsOnly)
	UChildActorComponent *InteractionActor;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *WaitTimelineCurve;
	UPROPERTY(EditInstanceOnly)
	TArray<FEnemyUnitSpawnDataList> EnemyList;
	UPROPERTY(EditInstanceOnly)
	AInteractionBattle *Battle;
	UPROPERTY(EditAnywhere)
	bool bRespawnAfterLoad;
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere)
	FOffLevelBattleData OverrideOffLevelBattleData;
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 15.0f;

	UPROPERTY()
	AInteractionDetector *InteractionDetector;
	UPROPERTY()
	class AAIController *AIController;

	bool bIsStartingBattle;
	FTimeline WaitTimeline;
	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
	TWeakObjectPtr<class UNavigationSystemV1> NavSystem;

	const float MinWaitPlayRate = 0.1f;
	const float MaxWaitPlayRate = 0.25f;
};
