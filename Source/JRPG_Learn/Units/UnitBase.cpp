// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ArrowComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Battle/BattleBase.h"
#include "../Battle/DamageText.h"
#include "../Controllers/JRPG_FunctionLibrary.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../UI/TargetIcon.h"
#include "TimerManager.h"

// Sets default values
AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	SkeletalMesh->SetupAttachment(RootComponent);

	TargetIconWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetIconWidget"));
	TargetIconWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIconWidget->SetDrawAtDesiredSize(true);
	TargetIconWidget->SetupAttachment(SkeletalMesh);

	CameraLookLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLookLocation"));
	CameraLookLocation->SetupAttachment(SkeletalMesh);

	ProjectileHitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileHitLocation"));
	ProjectileHitLocation->SetupAttachment(SkeletalMesh);

	FrontCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FrontCameraSpringArm"));
	FrontCameraSpringArm->SetupAttachment(SkeletalMesh);
	FrontCameraSpringArm->bDoCollisionTest = 0;
	FrontCameraSpringArm->bAutoActivate = 0;

	FrontCameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("FrontCameraActor"));
	FrontCameraActor->SetupAttachment(FrontCameraSpringArm);

	AttackerLocationSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("AttackerLocationSpringArm"));
	AttackerLocationSpringArm->SetupAttachment(SkeletalMesh);
	AttackerLocationSpringArm->bDoCollisionTest = 0;
	AttackerLocationSpringArm->bInheritPitch = 0;
	AttackerLocationSpringArm->bInheritYaw = 0;
	AttackerLocationSpringArm->bInheritRoll = 0;

	AttackerLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackerLocation"));
	AttackerLocation->SetupAttachment(AttackerLocationSpringArm);

	BackCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BackCameraSpringArm"));
	BackCameraSpringArm->SetupAttachment(RootComponent);
	BackCameraSpringArm->bDoCollisionTest = 0;
	BackCameraSpringArm->bInheritPitch = 0;
	BackCameraSpringArm->bInheritYaw = 0;
	BackCameraSpringArm->bInheritRoll = 0;

	BackCameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackCameraActor"));
	BackCameraActor->SetupAttachment(BackCameraSpringArm);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = (SkeletalMesh)? SkeletalMesh->GetAnimInstance() : nullptr; 
	PlayerController = Cast<AJRPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AUnitBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotationTimeline.TickTimeline(DeltaSeconds);
	MovementTimeline.TickTimeline(DeltaSeconds);
}

void AUnitBase::InitUnit(ABattleController *Controller)
{
	UE_LOG(LogTemp, Log, TEXT("AUnitBase::InitUnit"))
	BattleController = Controller;
	SetUnitStats();
	if (OnActionTimeAdded.IsBound())
	{
		OnActionTimeAdded.Broadcast(0);
	}
	PlayAnimMontage(IntroAnimMontage);
	TargetIconWidget->InitWidget();
	TargetIconUI = Cast<UTargetIcon>(TargetIconWidget);
	FTransform MeshTransform = SkeletalMesh->GetComponentTransform();
	InitialLocation = MeshTransform.GetLocation();
	InitialRotation = MeshTransform.GetRotation();
	SpawnedTransform = GetActorTransform();
	Battle->OnBattleRemoved.AddUObject(this, &AUnitBase::OnBattleRemovedHandler);
}

void AUnitBase::OnBattleRemovedHandler()
{
	Destroy();
}

void AUnitBase::SetBattle(ABattleBase * Battle_l)
{
	Battle = TWeakObjectPtr<ABattleBase>(Battle_l);
}

bool AUnitBase::IsRangeUnit()
{
	return UKismetSystemLibrary::IsValidClass(Projectile);
}

void AUnitBase::SetUnitStats()
{
	FUnitStats Stats = UJRPG_FunctionLibrary::GetUnitStats(Level, MaxLevel, InitialStats, FirstLevelStats, LastLevelStats);
	MaxHP = Stats.MaxHP;
	MaxMP = Stats.MaxMP;
	MinAttack = Stats.MinAttack;
	MaxAttack = Stats.MaxAttack;
	Defense = Stats.Defense;
	Speed = Stats.Speed;
	Hit = Stats.Hit;
	MinMagicalAttack = Stats.MinMagicalAttack;
	MaxMagicalAttack = Stats.MaxMagicalAttack;
	MagicDefense = Stats.MagicDefense;
	ActionTime = Stats.ActionTime;
}

void AUnitBase::AddActionTime(float Time)
{
	if (bIsStuned)
		return;

	if (OnActionTimeAdded.IsBound())
	{
		OnActionTimeAdded.Broadcast(Time);
	}
	float NewActionTime = CurrentActionTime + Time;
	if (NewActionTime >= ActionTime)
	{
		NewActionTime = 0;
	}
	CurrentActionTime = NewActionTime;
}

void AUnitBase::FocusOnTarget(FRotator Rotation)
{
	TargetRotation = Rotation;
	StartRotation = SkeletalMesh->GetComponentRotation();
	if (IsValid(RotationTimelineCurve))
	{
		FOnTimelineFloatStatic RotationTimelineUpdate;
		RotationTimelineUpdate.BindUObject(this, &AUnitBase::RotationTimelineUpdate);
		RotationTimeline.AddInterpFloat(RotationTimelineCurve, RotationTimelineUpdate);
		RotationTimeline.Play();
	}
}

void AUnitBase::RotationTimelineUpdate(const float Alpha)
{
	float TargetYaw = UKismetMathLibrary::Lerp(StartRotation.Yaw, TargetRotation.Yaw, Alpha);
	FRotator Rotation{StartRotation.Pitch, TargetYaw, StartRotation.Roll};
	SkeletalMesh->SetWorldRotation(Rotation);
}

void AUnitBase::MoveToTarget(FVector Location, bool bIsForward)
{
	TargetLocation = Location;
	StartLocation = SkeletalMesh->GetComponentLocation();
	PlayAnimMontage(bIsForward ? DashForwardAnimMontage : DashBackwardAnimMontage);
	float NewPlayRate = MoveSpeed / (TargetLocation - StartLocation).Size();
	MovementTimeline.SetPlayRate(NewPlayRate);
	if (IsValid(MovementTimelineCurve))
	{
		FOnTimelineFloatStatic MovementTimelineUpdate;
		FOnTimelineEventStatic MovementTimelineFinish;
		MovementTimelineUpdate.BindUObject(this, &AUnitBase::MovementTimelineUpdate);
		MovementTimelineFinish.BindUObject(this, &AUnitBase::MovementByTimelineFinished);
		MovementTimeline.AddInterpFloat(MovementTimelineCurve, MovementTimelineUpdate);
		MovementTimeline.SetTimelineFinishedFunc(MovementTimelineFinish);
		MovementTimeline.Play();
	}
}

void AUnitBase::MovementTimelineUpdate(const float Alpha)
{
	FVector Location = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	SkeletalMesh->SetWorldLocation(Location);
}

void AUnitBase::MovementByTimelineFinished()
{
	PlayAnimMontage(IdleAnimMontage);
	if (OnMovedToTarget.IsBound())
	{
		OnMovedToTarget.Broadcast();
		OnMovedToTarget.RemoveAll(this);
	}
}

void AUnitBase::AdjustAttackerLocation(AUnitBase *Unit)
{
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Unit->GetActorLocation());
	float Yaw = Rotation.Yaw + 180.0f;
	AttackerLocationSpringArm->SetRelativeRotation(FRotator{0, Yaw, 0});
}

void AUnitBase::StartTurn()
{
	EndTurn();
}

void AUnitBase::EndTurn()
{
	FocusOnTarget(InitialRotation.Rotator());
	if (OnTurnEnded.IsBound())
	{
		OnTurnEnded.Broadcast();
	}
}

void AUnitBase::SwitchToFrontCamera(bool bIsInstant)
{
	if(PlayerController->GetViewTarget() != FrontCameraActor->GetChildActor())
	{
		float BlendTime = bIsInstant ? 0.f : MaxBlendCameraSpeed;
		PlayerController->SetViewTargetWithBlend(FrontCameraActor->GetChildActor(), BlendTime, EViewTargetBlendFunction::VTBlend_Cubic, .0f, true);
	}
}

void AUnitBase::SetTargetIconVisibility(bool bVisibilityState)
{
	TargetIconWidget->SetVisibility(bVisibilityState);
	if (OnTargetIconShown.IsBound())
	{
		OnTargetIconShown.Broadcast(bVisibilityState);
	}
}

void AUnitBase::Heal(int HealAmount)
{
	int NewHP = UKismetMathLibrary::Clamp(CurrentHP + HealAmount, 0, MaxHP);
	SetHP(NewHP);
	FTransform HitTransform;
	HitTransform.SetLocation(ProjectileHitLocation->GetComponentLocation());
	ADamageText *DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, HitTransform);
	DamageText->CreateTextWidget(HealAmount, this, true);
}

void AUnitBase::IncreaseMP(int ManaAmount)
{
	int NewMP = UKismetMathLibrary::Clamp(CurrentMP + ManaAmount, 0, MaxMP);
	SetMP(NewMP);
	FTransform HitTransform;
	HitTransform.SetLocation(ProjectileHitLocation->GetComponentLocation());
	ADamageText *DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, HitTransform);
	DamageText->CreateTextWidget(ManaAmount, this, false, true);
}

void AUnitBase::ReceiveDamage(int Damage)
{
	int NewHP = UKismetMathLibrary::Clamp(CurrentHP - Damage, 0, MaxHP);
	SetHP(NewHP);
	if (!IsUnitDead() && Damage > 0)
	{
		PlayAnimMontage(bIsStuned ? StunAnimMontage : GetHitAnimMontage);
	}
}

void AUnitBase::UseMP(int ManaAmount)
{
	int NewMP = UKismetMathLibrary::Clamp(CurrentMP - ManaAmount, 0, MaxMP);
	SetMP(NewMP);
}

void AUnitBase::SetHP(int HealthAmount)
{
	CurrentHP = HealthAmount;
	if (OnHPSet.IsBound())
	{
		OnHPSet.Broadcast(CurrentHP);
	}
	if (IsUnitDead())
	{
		if (OnUnitDied.IsBound())
		{
			OnUnitDied.Broadcast(this);
		}
	}
}

void AUnitBase::SetMP(int ManaAmount)
{
	CurrentMP = ManaAmount;
	if (OnMPSet.IsBound())
	{
		OnMPSet.Broadcast(CurrentMP);
	}
}

void AUnitBase::Die()
{
	FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
       OnDied();
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, DieAnim->SequenceLength, false);
}

void AUnitBase::OnDied()
{
	FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
       Destroy();
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, DelayBeforeDie, false);
}

float AUnitBase::PlayAnimMontage(UAnimMontage *AnimMontage, float InPlayRate , FName StartSectionName)
{
	if( AnimMontage && AnimInstance )
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			if( StartSectionName != NAME_None )
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}
		}
	}
	return .0f;	
}

FTransform AUnitBase::GetProjectileSpawnTransform()
{
	FTransform ProjectileSpawnTransform = SkeletalMesh->GetSocketTransform(ProjectileSocketName);
	return ProjectileSpawnTransform;
}