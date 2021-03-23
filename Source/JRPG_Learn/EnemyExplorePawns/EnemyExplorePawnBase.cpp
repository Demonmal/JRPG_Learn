// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyExplorePawnBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "AIController.h"
#include "../Battle/InteractionBattle.h"
#include "../Interactables/InteractionDetector.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Controllers/JRPG_FunctionLibrary.h"

// Sets default values
AEnemyExplorePawnBase::AEnemyExplorePawnBase()
{
	InteractionActor = CreateDefaultSubobject<UChildActorComponent>("InteractionActor");
	InteractionActor->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AEnemyExplorePawnBase::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FOnTimelineEventStatic WaitTimelineFinish;
	WaitTimelineFinish.BindUObject(this, &AEnemyExplorePawnBase::OnWaitFinished);
	WaitTimeline.SetTimelineFinishedFunc(WaitTimelineFinish);
	WaitTimeline.SetFloatCurve(WaitTimelineCurve, "WaitCurve");
	EnableAI();
	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());
	InteractionDetector->OnInteracted.AddUObject(this, &AEnemyExplorePawnBase::OnInteractedHandler);
	InteractionDetector->OnLoaded.AddUObject(this, &AEnemyExplorePawnBase::OnLoadedHandler);
	PlayerController = Cast<AJRPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->OnGameStateChanged.AddUObject(this, &AEnemyExplorePawnBase::OnGameStateChangedHandler);
}

 void AEnemyExplorePawnBase::Tick(float DeltaSeconds)
 {
	Super::Tick(DeltaSeconds);
	WaitTimeline.TickTimeline(DeltaSeconds);
 }

void AEnemyExplorePawnBase::StartBattle()
{
	Battle->StartBattle(EnemyList, OverrideOffLevelBattleData);
	FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(GetWorld(), 0.1f, LatentActionInfo);
	Destroy();
}

void AEnemyExplorePawnBase::DisableAI()
{
	StopAI();
	if(!bIsStartingBattle)
	{
		UJRPG_FunctionLibrary::SetActorDisableState(this, true);
	}
}

void AEnemyExplorePawnBase::EnableAI()
{
	UJRPG_FunctionLibrary::SetActorDisableState(this, false);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	float PlayRate = 1.0f / UKismetMathLibrary::RandomFloatInRange(2.0f, 5.0f);
	WaitTimeline.SetPlayRate(PlayRate);
	WaitTimeline.PlayFromStart();
}

void AEnemyExplorePawnBase::StopAI()
{
	AIController->StopMovement();
	WaitTimeline.Stop();
}

void AEnemyExplorePawnBase::OnWaitFinished()
{
	FVector Origin = Battle->GetActorLocation();
	FNavLocation RandomPoint;
	if(NavSystem->GetRandomReachablePointInRadius(Origin, Battle->GetRadius() * 50.0f, RandomPoint))
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(RandomPoint.Location);
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
		AIController->MoveTo(MoveRequest);
	}
	WaitTimeline.PlayFromStart();
}

void AEnemyExplorePawnBase::OnInteractedHandler()
{
	bIsStartingBattle = true;
	StartBattle();
}

void AEnemyExplorePawnBase::OnLoadedHandler(bool bShouldLoadEnemyPawns)
{
	if (bRespawnAfterLoad && bShouldLoadEnemyPawns)
	{
		InteractionDetector->SetCanInteractState(true);
	}
	else
	{
		Destroy();
	}
}

void AEnemyExplorePawnBase::OnGameStateChangedHandler(EGameState GameState)
{
	switch (GameState)
	{
	case EGameState::Explore:
		EnableAI();
		break;
	case EGameState::BattleTransition:
		DisableAI();
		break;
	case EGameState::Pause:
		StopAI();
		break;
	}
}