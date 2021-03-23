// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Units/UnitBase.h"

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;
	ProjectileMoveTimeline.TickTimeline(DeltaTime);
}

void AProjectileBase::SetTargetUnit(AUnitBase *Unit)
{
	TargetUnit = TWeakObjectPtr<AUnitBase>(Unit);
}

void AProjectileBase::MoveProjectileToTarget()
{
	TargetLocation = TargetUnit->GetProjectileHitLocation()->GetComponentLocation();
	float PlayRate = Speed / (GetActorLocation() - TargetLocation).Size();
	ProjectileMoveTimeline.SetPlayRate(PlayRate);
	FOnTimelineFloatStatic ProjectileMoveTimelineUpdate;
	FOnTimelineEventStatic ProjectileMoveTimelineFinish;
	ProjectileMoveTimelineUpdate.BindUObject(this, &AProjectileBase::ProjectileMoveTimelineUpdate);
	ProjectileMoveTimelineFinish.BindUObject(this, &AProjectileBase::ProjectileMoveTimelineFinish);
	ProjectileMoveTimeline.AddInterpFloat(ProjectileMoveCurve, ProjectileMoveTimelineUpdate);
	ProjectileMoveTimeline.SetTimelineFinishedFunc(ProjectileMoveTimelineFinish);
	ProjectileMoveTimeline.Play();
}

void AProjectileBase::ProjectileMoveTimelineUpdate(const float Alpha)
{	
	FVector NewLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, Speed);
	SetActorLocation(NewLocation);
}

void AProjectileBase::ProjectileMoveTimelineFinish()
{
	if(OnProjectileHit.IsBound())
	{
		OnProjectileHit.Broadcast();
	}
	PlayHitSound();
	DestroyProjectile();
}

void AProjectileBase::DestroyProjectile()
{
	Destroy();
}

void AProjectileBase::PlayHitSound()
{
	if (IsValid(HitSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
	}
}