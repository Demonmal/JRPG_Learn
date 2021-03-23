// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/DataTypes.h"
#include "Components/TimelineComponent.h"
#include "UnitBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPSetSignature, int)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDiedSignature, AUnitBase *)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMPSetSignature, int)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionTimeAddedSignature, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetIconShownSignature, bool)
DECLARE_MULTICAST_DELEGATE(FOnTurnEndedSignature)
DECLARE_MULTICAST_DELEGATE(FOnTurnStartedSignature)
DECLARE_MULTICAST_DELEGATE(FOnMovedToTargetSignature)

class USpringArmComponent;
class UChildActorComponent;
class USceneComponent;
class UAnimMontage;
class UAnimSequence;
class ABattleController;
class AJRPG_PlayerController;
class AProjectileBase;
class USkeletalMeshComponent;
class UAnimInstance;
class ABattleBase;
class UArrowComponent;

UCLASS()
class JRPG_LEARN_API AUnitBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitBase();

	FOnHPSetSignature OnHPSet;
	FOnUnitDiedSignature OnUnitDied;
	FOnMPSetSignature OnMPSet;
	FOnActionTimeAddedSignature OnActionTimeAdded;
	FOnTargetIconShownSignature OnTargetIconShown;
	FOnTurnEndedSignature OnTurnEnded;
	FOnTurnStartedSignature OnTurnStarted;
	FOnMovedToTargetSignature OnMovedToTarget;

	virtual void Tick(float DeltaSeconds) override;

	virtual void InitUnit(ABattleController *Controller);
	virtual void SetUnitStats();
	virtual void AddActionTime(float Time);
	virtual void FocusOnTarget(FRotator Rotation);
	virtual void MoveToTarget(FVector Location, bool bIsForward);
	virtual void AdjustAttackerLocation(AUnitBase *Unit);
	virtual void StartTurn();
	virtual void EndTurn();
	virtual void SwitchToFrontCamera(bool bIsInstant);
	virtual void SetTargetIconVisibility(bool bVisibilityState);
	virtual void Heal(int HealAmount);
	virtual void IncreaseMP(int ManaAmount);
	virtual void ReceiveDamage(int Damage);
	virtual void UseMP(int ManaAmount);
	virtual void SetHP(int HealthAmount);
	virtual void SetMP(int ManaAmount);
	virtual void Die();
	virtual void OnDied();
	virtual FTransform GetProjectileSpawnTransform();

	void RotationTimelineUpdate(const float Alpha);
	void MovementTimelineUpdate(const float Alpha);
	void MovementByTimelineFinished();

	bool IsRangeUnit();

	float PlayAnimMontage(UAnimMontage *AnimMontage, float InPlayRate = 1.0f, FName StartSectionName = "");

	FORCEINLINE bool IsUnitDead() const { return CurrentHP <= 0; }
	FORCEINLINE bool IsStuned() const { return bIsStuned; }
	FORCEINLINE bool IsMoving() const { return bIsMoving; }
	FORCEINLINE bool IsMovingForward() const { return bIsMovingForward; }
	FORCEINLINE float GetActionTime() const { return ActionTime; }
	FORCEINLINE float GetCurrentActionTime() const { return CurrentActionTime; }
	FORCEINLINE int GetMaxLevel() const { return MaxLevel; }
	FORCEINLINE int GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE int GetCurrentMP() const { return CurrentMP; }
	FORCEINLINE int GetMaxHP() const { return MaxHP; }
	FORCEINLINE int GetMaxMP() const { return MaxMP; }
	FORCEINLINE int GetUnitLevel() const { return Level; }
	FORCEINLINE int GetMinAttack() const { return MinAttack; }
	FORCEINLINE int GetMaxAttack() const { return MaxAttack; }
	FORCEINLINE int GetMinMagicalAttack() const { return MinMagicalAttack; }
	FORCEINLINE int GetMaxMagicalAttack() const { return MaxMagicalAttack; }
	FORCEINLINE int GetDefense() const { return Defense; }
	FORCEINLINE int GetMagicDefense() const { return MagicDefense; }
	FORCEINLINE int GetHit() const { return Hit; }
	FORCEINLINE int GetSpeed() const { return Speed; }
	FORCEINLINE FUnitStats GetInitialStats() const { return InitialStats; }
	FORCEINLINE FUnitStats GetFirstLevelStats() const { return FirstLevelStats; }
	FORCEINLINE FUnitStats GetLastLevelStats() const { return LastLevelStats; }
	FORCEINLINE UAnimSequence *GetIdleAnim() const { return IdleAnim; }
	FORCEINLINE UAnimSequence *GetDieAnim() const { return DieAnim; }
	FORCEINLINE USceneComponent *GetCameraLookLocation() const { return CameraLookLocation; }
	FORCEINLINE USceneComponent *GetProjectileHitLocation() const { return ProjectileHitLocation; }
	FORCEINLINE TSubclassOf<AProjectileBase> GetProjectileClass() const { return Projectile; }
	FORCEINLINE FText GetTitle() const { return Title; }
	FORCEINLINE USkeletalMeshComponent *GetSkeletalMesh() const { return SkeletalMesh; }
	FORCEINLINE UArrowComponent *GetAttackerLocation() const { return AttackerLocation; }
	FORCEINLINE UTexture2D *GetThumbnail() const { return Thumbnail; }
	FORCEINLINE UAnimMontage *GetItemUseAnimMontage() const { return ItemUseAnimMontage; }
	FORCEINLINE UAnimMontage *GetAttackAnimMontage() const { return AttackAnimMontage; }
	FORCEINLINE UAnimInstance *GetAnimInstance() const { return AnimInstance; }
	FORCEINLINE FTransform GetSpawnedTransform() const { return SpawnedTransform; }
	FORCEINLINE FVector GetInitialLocation() const { return InitialLocation; }
	FORCEINLINE AUnitBase *GetCurrentTarget() const { return CurrentTarget.Get(); }

	FORCEINLINE void SetLevel(int Level_l) {Level = Level_l;}
	void SetBattle(ABattleBase * Battle_l);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBattleRemovedHandler();

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USkeletalMeshComponent *SkeletalMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UWidgetComponent *TargetIconWidget;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent *CameraLookLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent *ProjectileHitLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent *FrontCameraSpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UChildActorComponent *FrontCameraActor;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent *AttackerLocationSpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UArrowComponent *AttackerLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent *BackCameraSpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UChildActorComponent *BackCameraActor;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *IntroAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *IdleAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimSequence *IdleAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimSequence *DieAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *DashForwardAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *DashBackwardAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *AttackAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *GetHitAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *ItemUseAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *StunAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage *VictoryAnimMontage;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *RotationTimelineCurve;
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *MovementTimelineCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FUnitStats InitialStats;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FUnitStats FirstLevelStats;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FUnitStats LastLevelStats;

	UPROPERTY(EditDefaultsOnly)
	FText Title;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D *Thumbnail;
	UPROPERTY(EditDefaultsOnly)
	int MaxLevel;
	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> Projectile;

	UPROPERTY()
	UAnimInstance *AnimInstance;

	TWeakObjectPtr<ABattleController> BattleController;
	TWeakObjectPtr<ABattleBase> Battle;
	TWeakObjectPtr<AUnitBase> CurrentTarget;
	TWeakObjectPtr<class UTargetIcon> TargetIconUI;
	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;

	int MaxHP;
	int MaxMP;
	float ActionTime;
	int MinAttack;
	int MaxAttack;
	int Defense;
	int Speed;
	int Hit;
	int MinMagicalAttack;
	int MaxMagicalAttack;
	int MagicDefense;
	int CurrentHP;
	int CurrentMP;
	float CurrentActionTime;
	FQuat InitialRotation;
	FRotator StartRotation;
	FRotator TargetRotation;
	FVector InitialLocation;
	FVector StartLocation;
	FVector TargetLocation;
	FTransform SpawnedTransform;
	int Level;

	bool bIsStuned;
	bool bIsMoving;
	bool bIsMovingForward;

	float DelayBeforeDie = 1.f;
	float MaxBlendCameraSpeed = 0.5f;
	FTimeline RotationTimeline;
	FTimeline MovementTimeline;

	const FName ProjectileSocketName = "ProjectileSocket";
};
