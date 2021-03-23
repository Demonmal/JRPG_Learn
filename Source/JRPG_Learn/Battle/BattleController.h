// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/Enums.h"
#include "../Data/DataTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BattleController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnTurnChangedSignature, int)
DECLARE_MULTICAST_DELEGATE_OneParam(OnActionTimeAddedSignature, float)

class UAudioComponent;
class AJRPG_PlayerController;
class USoundBase;
class ABattleTransitionBase;
class AUnitBase;
class ABattleBase;
class ABattleSkillBase;
class UBattleUI;
class APlayerUnitBase;
class UVictoryDialogue;
class ULevelUpDialogue;
class UDefeatDialogue;
class AItemBase;

UCLASS()
class JRPG_LEARN_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleController();

	OnTurnChangedSignature OnTurnChanged;
	OnActionTimeAddedSignature OnActionTimeAdded;

	void InitBattle(ABattleBase* Battle, AJRPG_PlayerController *Controller);
	void EnemyPrepareForSkill(TSubclassOf<ABattleSkillBase> BattleSkill);
	void AdjustCamera(APlayerUnitBase *PlayerUnit, AUnitBase *TargetUnit, bool bIsInstant);
	void UpdateUnitBattleDetails(AUnitBase *AttackingUnit, AUnitBase *TargetUnit);
	void Attack(AUnitBase *TargetUnit);
	void ShowSkills(APlayerUnitBase *PlayerUnit);
	void ShowItemInventory();
	void Flee(APlayerUnitBase *PlayerUnit);
	void EnemyUseSkill();
	bool DealDamage(int PureDamage, float DamageMultiplier);
	void SwitchToBattleMode();
	void SwitchToExploreMode();
	void StartBattle();
	void IncreaseTurnCount();
	void SetReadyUnits();

	FORCEINLINE AJRPG_PlayerController* GetPlayerController() const { return PlayerController.Get(); }
	FORCEINLINE ETurnType GetTurnType() const { return TurnType; }
	FORCEINLINE bool GetShowTurnOrderUI() const { return bShowTurnOrderUI; }
	FORCEINLINE ABattleBase *GetCurrentBattle() const { return CurrentBattle.Get(); }
	FORCEINLINE AUnitBase *GetCurrentAttackingUnit() const { return CurrentAttackingUnit.Get(); }
	FORCEINLINE AUnitBase *GetCurrentTargetUnit() const { return CurrentTargetUnit.Get(); }

	FName GetBattleLevelName();
	TSubclassOf<ABattleTransitionBase> GetBattleTransitionClass();

protected:
	virtual void BeginPlay() override;

private:

	void PlayExploreTheme();
	void PlayBattleTheme();
	void StartTransition();
	void ChangeMapForBattle();
	bool SwitchToStaticCamera();
	void OnUnitsTurnStarted();
	void OnUnitsTurnEnded();
	void OnBattleOver();
	void PlayerWon();
	void EnemyWon();
	void RangeAttack();
	void MeleeAttack();
	void AddUsableItemsToPlayerInventory();
	void AddMiscItemsToPlayerInventory();
	void AddEquipmentToPlayerInventory();
	void ShowLeveledUpUnit();
	bool IsUnitHasEnoughMP(TSubclassOf<ABattleSkillBase> Skill);
	void UseSkill(AUnitBase *TargetUnit);
	void UseItem(AUnitBase *TargetUnit, TSubclassOf<APlayerUnitBase> UnitClass);
	void MoveToTarget(FVector TargetLocation, void (ABattleController::* InFunc)(), bool bIsForward);
	TMap<TSubclassOf<APlayerUnitBase>, int> GetLeveledUpUnits();

	UFUNCTION()
	void OnTurnEndedHandler();
	UFUNCTION()
	void OnUnitMovedToTargetHandler();
	UFUNCTION()
	void OnUnitMovedBackHandler();
	UFUNCTION()
	void OnMeleeAttackAnimMontageNotified(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload);
	UFUNCTION()
	void OnRangeAttackAnimMontageNotified(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload);
	UFUNCTION()
	void OnProjectileHitHandler();
	UFUNCTION()
	void OnContinueClickedHandler();
	UFUNCTION()
	void OnLevelUpContinueClickedHandler();

	UFUNCTION()
	void OnItemSelectedHandler(TSubclassOf<AItemBase> SelectedItem);
	UFUNCTION()
	void OnItemUseCancelledHandler();
	UFUNCTION()
	void OnItemInventoryClosedHandler();
	UFUNCTION()
	void OnItemUsedHandler();
	UFUNCTION()
	void OnItemUsed();

	UFUNCTION()
	void OnSkillSelectedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill);
	UFUNCTION()
	void OnSkillUIClosed();
	UFUNCTION()
	void OnSkillUsedUIHandler();
	UFUNCTION()
	void OnSkillUseCancelled();
	UFUNCTION()
	void OnSkillUsedHandler();

	TArray<AUnitBase *> GetReadyUnitsBasedOnAT(float& ClosestActionTime);
	TArray<AUnitBase *> GetReadyUnitsBasedOnSpeed();
	AUnitBase *GetUnitWithHighestSpeed(TArray<AUnitBase *> Units);

	UFUNCTION()
	void OnBattleOverHandler(EBattleResult BattleResult_l);

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
	TSubclassOf<UBattleUI> BattleUIClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UVictoryDialogue> VictoryDialogueClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelUpDialogue> LevelUpDialogueClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDefeatDialogue> DefeatDialogueClass;
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

	UPROPERTY()
	UBattleUI *BattleUI;
	UPROPERTY()
	TArray<AUnitBase *> ReadyToAttackUnits;

	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
	TWeakObjectPtr<ABattleBase> CurrentBattle;
	TWeakObjectPtr<ABattleTransitionBase> CurrentBattletransition;
	TWeakObjectPtr<AUnitBase> CurrentAttackingUnit;
	TWeakObjectPtr<AUnitBase> CurrentTargetUnit;
	TWeakObjectPtr<class AUsableItemBase> CurrentUsableItem;
	TWeakObjectPtr<class ABattleSkillBase> CurrentSkill;
	TWeakObjectPtr<APawn> ExploreCharacter;
	TMap<TSubclassOf<APlayerUnitBase>, int> LeveledUpUnits;

	int AttackUnitIndex;
	int LeveledUpUnitIndex;
	int CurrentTurn;
	EBattleResult BattleResult;
	bool bIsBattleOver;
	bool bIsPlayerVictory;
	bool bRangeAttackAnimHasEnded;
	bool bHasProjectileHit;
	FLatentActionInfo LatentActionInfo;

	const FName AttackNotifyName = "Attack";
};
