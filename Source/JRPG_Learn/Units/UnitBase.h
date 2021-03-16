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

	virtual void InitUnit(ABattleController *Controller);
	virtual void SetUnitStats();
	void AddActionTime(float Time);
	void PlayAnimationMontage(UAnimMontage *Montage);
	void FocusOnTarget(FRotator Rotation);
	void MoveToTarget(FVector Location, bool bIsForward);
	void AdjustAttackerLocation(AUnitBase *Unit);
	void StartTurn();
	void EndTurn();
	void SwitchToFrontCamera(bool bIsInstant);
	void SetTargetIconVisibility(bool bVisibilityState);
	void Heal(int HealAmount);
	void IncreaseMP(int ManaAmount);
	void ReceiveDamage(int Damage);
	void UseMP(int ManaAmount);
	void SetHP(int HealthAmount);
	void SetMP(int ManaAmount);
	void Die();
	void OnDied();

	void RotationTimelineUpdate(const float Alpha);
	void MovementTimelineUpdate(const float Alpha);
	void MovementByTimelineFinished();

	FORCEINLINE bool IsUnitDead() const { return CurrentHP <= 0; }
	FORCEINLINE bool IsStuned() const { return bIsStuned; }
	FORCEINLINE bool IsMoving() const { return bIsMoving; }
	FORCEINLINE bool IsMovingForward() const { return bIsMovingForward; }
	FORCEINLINE int GetMaxLevel() const { return MaxLevel; }
	FORCEINLINE int GetMaxHP() const { return MaxHP; }
	FORCEINLINE int GetMaxMP() const { return MaxMP; }
	FORCEINLINE FUnitStats GetInitialStats() const { return InitialStats; }
	FORCEINLINE FUnitStats GetFirstLevelStats() const { return FirstLevelStats; }
	FORCEINLINE FUnitStats GetLastLevelStats() const { return LastLevelStats; }
	FORCEINLINE UAnimSequence *GetIdleAnim() const { return IdleAnim; }
	FORCEINLINE UAnimSequence *GetDieAnim() const { return DieAnim; }
	FORCEINLINE USceneComponent *GetCameraLookLocation() const { return CameraLookLocation; }
	FORCEINLINE FText GetTitle() const { return Title; }

	FORCEINLINE UAnimMontage * GetItemUseAnimMontage() const { return ItemUseAnimMontage; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent *SkeletalMesh;
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
	class UArrowComponent *AttackerLocation;
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

	TWeakObjectPtr<ABattleController> BattleController;
	TWeakObjectPtr<AUnitBase> CurrentTarget;
	TWeakObjectPtr<class UTargetIcon> TargetIconUI;

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
};
