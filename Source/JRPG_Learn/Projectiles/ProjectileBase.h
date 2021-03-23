// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ProjectileBase.generated.h"

class USoundBase;
class AUnitBase;
class UParticleSystem;

DECLARE_MULTICAST_DELEGATE(OnProjectileHitSignature)

UCLASS()
class JRPG_LEARN_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:	

	OnProjectileHitSignature OnProjectileHit;
	
	virtual void Tick(float DeltaTime) override;
	virtual void MoveProjectileToTarget();

	void SetTargetUnit(AUnitBase *Unit);

protected:

	virtual void DestroyProjectile();
	virtual void PlayHitSound();

	UFUNCTION()
	void ProjectileMoveTimelineUpdate(const float Alpha);
	UFUNCTION()
	void ProjectileMoveTimelineFinish();

	UPROPERTY(EditDefaultsOnly)
	float Speed;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem *HitEffect;
	UPROPERTY(EditDefaultsOnly)
	USoundBase *HitSound;
	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat *ProjectileMoveCurve;

	FTimeline ProjectileMoveTimeline;
	float DeltaSeconds;
	FVector TargetLocation;
	TWeakObjectPtr<AUnitBase> TargetUnit;
};
