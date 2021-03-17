// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/Enums.h"
#include "../Data/DataTypes.h"
#include "BattleController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnTurnChangedSignature, int)
DECLARE_MULTICAST_DELEGATE_OneParam(OnActionTimeAddedSignature, float)

class UAudioComponent;
class AJRPG_PlayerController;
class USoundBase;
class ABattleTransitionBase;
class AUnitBase;

UCLASS()
class JRPG_LEARN_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleController();

	OnTurnChangedSignature OnTurnChanged;
	OnActionTimeAddedSignature OnActionTimeAdded;

	FORCEINLINE AJRPG_PlayerController* GetPlayerController() const { return PlayerController.Get(); }
	FORCEINLINE ETurnType GetTurnType() const { return TurnType; }

protected:
	virtual void BeginPlay() override;

private:

	void PlayExploreTheme();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *BattleThemeAudio;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *VictoryThemeAudio;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *ExploreThemeAudio;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase *ExploreTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase *BattleTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase *VictoryTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase *DefeatTheme;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABattleTransitionBase> BattleTransition;
	UPROPERTY(EditAnywhere)
	bool bShouldUnitsMoveToTarget;
	UPROPERTY(EditAnywhere)
	bool bShowTurnOrderUI;
	UPROPERTY(EditAnywhere)
	ETurnType TurnType;
	UPROPERTY(EditAnywhere)
	FName MainMenuMapName;
	UPROPERTY(EditAnywhere)
	FOffLevelBattleData OffLevelBattleData;

	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
	TWeakObjectPtr<class ABattleBase> CurrentBattle;
	TWeakObjectPtr<ABattleTransitionBase> CurrentBattletransition;
	TWeakObjectPtr<AUnitBase> CurrentAttackingUnit;
	TWeakObjectPtr<AUnitBase> CurrentTargetUnit;
	TWeakObjectPtr<class UBattleUI> BattleUI;
	TWeakObjectPtr<class AUsableItemBase> CurrentUsableItem;
	TWeakObjectPtr<class ABattleSkillBase> CurrentSkill;
	TWeakObjectPtr<APawn> ExploreCharacter;
	TArray<TWeakObjectPtr<AUnitBase>> ReadyToAttackUnits;
	TMap<TSubclassOf<class APlayerUnitBase>, int> LeveledUpUnits;

	int AttackUnitIndex;
	int LeveledUpUnitIndex;
	int CurrentTurn;
	EBattleResult BattleResult;
	bool bIsBattleOver;
	bool bIsPlayerVictory;
	bool bRangeAttackAnimHasEnded;
	bool bHasProjectileHit;
};
