// Fill out your copyright notice in the Description page of Project Settings.

#include "JRPG_CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NavigationSystem/Public/NavAreas/NavArea_Obstacle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "../Items/Equipment/EquipmentMesh/EquipmentMeshBase.h"
#include "AIController.h"
#include "AITypes.h"

AJRPG_CharacterBase::AJRPG_CharacterBase() : ACharacter::ACharacter()
{
	PrimaryActorTick.bCanEverTick = 1;

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::Disabled;

	RootComponent = GetCapsuleComponent();

	GetCapsuleComponent()->AreaClass = FNavigationSystem::GetDefaultObstacleArea();

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = 1;
	SpringArmComponent->ProbeChannel = ECollisionChannel::ECC_Visibility;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	auto MeshComponent = GetMesh();

	BattleDetector = CreateDefaultSubobject<USphereComponent>(TEXT("BattleDetector"));
	BattleDetector->SetupAttachment(MeshComponent);
	BattleDetector->AreaClass = FNavigationSystem::GetDefaultObstacleArea();

	InteractionArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionArea"));
	InteractionArea->SetupAttachment(MeshComponent);
	InteractionArea->AreaClass = FNavigationSystem::GetDefaultObstacleArea();

	PartyMember1SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PartyMember1SpringArm"));
	PartyMember1SpringArm->SetupAttachment(RootComponent);
	PartyMember1SpringArm->ProbeChannel = ECollisionChannel::ECC_Visibility;

	PartyMember2SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PartyMember2SpringArm"));
	PartyMember2SpringArm->SetupAttachment(RootComponent);
	PartyMember2SpringArm->ProbeChannel = ECollisionChannel::ECC_Visibility;

	PartyMember1Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PartyMember1Arrow"));
	PartyMember1Arrow->SetupAttachment(PartyMember1SpringArm);
	PartyMember2Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PartyMember2Arrow"));
	PartyMember2Arrow->SetupAttachment(PartyMember2SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = 1;
}

void AJRPG_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
}

void AJRPG_CharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	bool IsAi = AIController != nullptr;
	if (!bIsControllingByAI || !PartyLeader.IsValid())
		return;
	float DistanceToPartyLeader = (PartyLeader->GetActorLocation() - GetActorLocation()).Size();
	if (DistanceToPartyLeader <= MinDistanceToPartyLeader)
		return;
	MoveToPartyLeader();
}

void AJRPG_CharacterBase::SetAI(bool state)
{
	bIsControllingByAI = state;
}

void AJRPG_CharacterBase::SetPartyLeader(AJRPG_CharacterBase *Leader, int Index)
{
	if (!IsValid(Leader))
		return;
	PartyLeader = Leader;
	PlayerIndex = Index;
	SetAI(true);
	if(!AIController && bIsControllingByAI)
	{
		this->SpawnDefaultController();
		AIController = Cast<AAIController>(GetController());
	}
	AIController->Possess(this);
	MoveToPartyLeader();
}

void AJRPG_CharacterBase::MoveToPartyLeader()
{
	if(!AIController)
		return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(PartyLeader->GetPartyMemberLocation(PlayerIndex));
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	AIController->MoveTo(MoveRequest);
}

FVector AJRPG_CharacterBase::GetPartyMemberLocation(int Index) const
{
	FVector Result{FVector::ZeroVector};
	if (Index == 1)
	{
		Result = PartyMember1Arrow->GetComponentLocation();
	}
	else if (Index == 2)
	{
		Result = PartyMember2Arrow->GetComponentLocation();
	}
	return Result;
}

void AJRPG_CharacterBase::RemoveAllEquipmentMesh()
{
	TArray<EEquipmentType> EquipmentKeys;
	EquipmentMeshList.GetKeys(EquipmentKeys);
	for (const auto &EquipmentType : EquipmentKeys)
	{
		RemoveEquipmentMesh(EquipmentType);
	}
}

void AJRPG_CharacterBase::RemoveEquipmentMesh(EEquipmentType EquipmentType)
{
	if (!EquipmentMeshList.Contains(EquipmentType))
		return;
	AEquipmentMeshBase *EquipmentMesh = EquipmentMeshList[EquipmentType];
	if (IsValid(EquipmentMesh))
	{
		EquipmentMesh->EquipmentOff(GetMesh());
		EquipmentMesh->Destroy();
	}
	EquipmentMeshList.Remove(EquipmentType);
}

void AJRPG_CharacterBase::ShowEquipmentMesh(EEquipmentType EquipmentType, TSubclassOf<AEquipmentMeshBase> EquipmentMeshClass)
{
	AEquipmentMeshBase *EquipmentMesh = Cast<AEquipmentMeshBase>(GetWorld()->SpawnActor(EquipmentMeshClass));
	EquipmentMesh->SetPlayerUnit(PlayerUnit);
	EquipmentMeshList.Add(EquipmentType, EquipmentMesh);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	EquipmentMesh->AttachToComponent(GetMesh(), AttachmentRules, EquipmentMesh->GetExploreSocketName());
	EquipmentMesh->EquipmentOn(GetMesh());
}