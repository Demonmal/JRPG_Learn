// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBase.h"
#include "BattleController.h"
#include "UnitSpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "../Units/EnemyUnits/EnemyUnitBase.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Controllers/JRPG_GameInstance.h"
#include "../Controllers/JRPG_FunctionLibrary.h"

ABattleBase::ABattleBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	BattleIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("BattleIcon"));
	BattleIcon->SetupAttachment(RootComponent);

	PlayerDetector = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerDetector"));
	PlayerDetector->bHiddenInGame = 1;
	PlayerDetector->SetupAttachment(RootComponent);
}

void ABattleBase::BeginPlay()
{
	Super::BeginPlay();
	InitPlayerSpawnLocations();
	PlayerController = Cast<AJRPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(BattleController))
	{
		UJRPG_GameInstance *GameInstance = Cast<UJRPG_GameInstance>(GetWorld()->GetGameInstance());
		checkf(IsValid(GameInstance), TEXT("ABattleBase::BeginPlay -> Game Instance isn't valid"));
		BattleController = GameInstance->GetBattleController();
		checkf(IsValid(BattleController), TEXT("ABattleBase::BeginPlay -> Battle Controller is null"));
	}
}

void ABattleBase::InitPlayerSpawnLocations()
{
	for (auto SpawnLocation : PlayerSpawnLocations)
	{
		SpawnLocation->Init(this);
	}
}

void ABattleBase::InitBattle()
{
	PlayerController->SetBlockPlayerUnitState(true);
	SpawnPlayerUnits();
	SpawnEnemyUnits();
	BattleController->InitBattle(this, PlayerController.Get());
}

void ABattleBase::SpawnPlayerUnits()
{
	PlayerUnitLevels.Empty();
	TArray<TSubclassOf<APlayerUnitBase>> PartyMembers = PlayerController->GetPartyMembers();
	for (int i{0}; i < PartyMembers.Num(); i++)
	{
		if (i < PlayerSpawnLocations.Num() && IsValid(PlayerSpawnLocations[i]))
		{
			APlayerUnitBase *PlayerDefault = Cast<APlayerUnitBase>(PartyMembers[i]->GetDefaultObject());
			FPlayerUnitData PlayerUnitData;
			PlayerController->TryGetUnitDataByPlayer(PartyMembers[i], PlayerUnitData);
			int Level = UJRPG_FunctionLibrary::GetLevel(PlayerUnitData.Exp, PlayerDefault->GetExpExponent(), PlayerDefault->GetMaxLevel());
			PlayerUnitLevels.Add(PartyMembers[i], Level);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(PlayerSpawnLocations[i]->GetActorLocation());
			SpawnTransform.SetRotation(PlayerSpawnLocations[i]->GetActorQuat());
			APlayerUnitBase *Unit = GetWorld()->SpawnActor<APlayerUnitBase>(PartyMembers[i], SpawnTransform);
			Unit->SetLevel(Level);
			Unit->SetBattle(this);
			PlayerUnits.Add(Unit);
		}
	}
	SetPlayerUnitsDisableState(true);
}

void ABattleBase::SpawnEnemyUnits()
{
	UsableItemDrops.Empty();
	TotalExpReward = 0;
	TotalGoldReward = 0;
	UE_LOG(LogTemp, Log, TEXT("SpawnEnemyUnits"))
	for (int i{0}; i < EnemySpawnLocations.Num(); i++)
	{
		EnemySpawnLocations[i]->Init(this);
		TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> EnemySpawnData = GetEnemySpawnData(i);
		TSubclassOf<AEnemyUnitBase> EnemyClass;
		int EnemyLevel;
		GetRandomEnemy(EnemySpawnData, EnemyClass, EnemyLevel);
		if (UKismetSystemLibrary::IsValidClass(EnemyClass))
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(EnemySpawnLocations[i]->GetActorLocation());
			SpawnTransform.SetRotation(EnemySpawnLocations[i]->GetActorQuat());
			AEnemyUnitBase *Unit = GetWorld()->SpawnActor<AEnemyUnitBase>(EnemyClass, SpawnTransform);
			Unit->SetLevel(EnemyLevel);
			Unit->SetBattle(this);
			EnemyUnits.Add(Unit);
			TotalExpReward += Unit->GetExpReward() * EnemyLevel;
			TotalGoldReward += Unit->GetGoldReward() * EnemyLevel;
			DropUsableItems(Unit);
			DropMiscItems(Unit);
			DropEquipment(Unit);
		}
	}
	SetEnemyUnitsDisableState(true);
}

void ABattleBase::DropUsableItems(AEnemyUnitBase *Unit)
{
	for (const auto &Pair : Unit->GetUsableItemDrops())
	{
		if (Pair.Value >= UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f))
		{
			if (UsableItemDrops.Contains(Pair.Key))
			{
				UsableItemDrops[Pair.Key] += 1;
			}
			else
			{
				UsableItemDrops.Add(Pair.Key, 1);
			}
		}
	}
}

void ABattleBase::DropMiscItems(AEnemyUnitBase *Unit)
{
	for (const auto &Pair : Unit->GetMiscItemDrops())
	{
		if (Pair.Value >= UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f))
		{
			if (MiscItemDrops.Contains(Pair.Key))
			{
				MiscItemDrops[Pair.Key] += 1;
			}
			else
			{
				MiscItemDrops.Add(Pair.Key, 1);
			}
		}
	}
}

void ABattleBase::DropEquipment(AEnemyUnitBase *Unit)
{
	for (const auto &Pair : Unit->GetEquipmentDrops())
	{
		if (Pair.Value >= UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f))
		{
			if (EquipmentDrops.Contains(Pair.Key))
			{
				EquipmentDrops[Pair.Key] += 1;
			}
			else
			{
				EquipmentDrops.Add(Pair.Key, 1);
			}
		}
	}
}

void ABattleBase::InitUnits()
{
	DeadPlayerUnits.Empty();
	UE_LOG(LogTemp, Log, TEXT("InitUnits"))
	for (auto Unit : PlayerUnits)
	{
		UJRPG_FunctionLibrary::SetActorDisableState(Cast<AActor>(Unit), false);
		InitPlayerUnit(Unit);
	}
	for (auto Unit : EnemyUnits)
	{
		UJRPG_FunctionLibrary::SetActorDisableState(Cast<AActor>(Unit), false);
		Unit->InitUnit(BattleController);
		Unit->OnUnitDied.AddUObject(this, &ABattleBase::OnEnemyUnitDiedHandler);
	}
}

void ABattleBase::InitPlayerUnit(AUnitBase *Unit)
{
	Unit->InitUnit(BattleController);
	Unit->OnUnitDied.AddUObject(this, &ABattleBase::OnPlayerUnitDiedHandler);
}

void ABattleBase::OnPlayerUnitDiedHandler(AUnitBase *Unit)
{
	TSubclassOf<APlayerUnitBase> PlayerUnitClass = Unit->StaticClass();
	DeadPlayerUnits.Add(PlayerUnitClass, Unit->GetSpawnedTransform());
	FPlayerUnitData PlayerUnitData;
	if (PlayerController->TryGetUnitDataByPlayer(PlayerUnitClass, PlayerUnitData))
	{
		PlayerUnitData.CurrentHP = 0;
		PlayerUnitData.CurrentMP = 0;
	}
	PlayerUnits.Remove(Cast<APlayerUnitBase>(Unit));
	if (PlayerUnits.Num() == 0)
	{
		if (OnBattleOver.IsBound())
		{
			OnBattleOver.Broadcast(EBattleResult::EnemyWon);
		}
	}
}

void ABattleBase::OnEnemyUnitDiedHandler(AUnitBase *Unit)
{
	EnemyUnits.Remove(Cast<AEnemyUnitBase>(Unit));
	if (EnemyUnits.Num() == 0)
	{
		if (OnBattleOver.IsBound())
		{
			OnBattleOver.Broadcast(EBattleResult::PlayerWon);
		}
	}
}

void ABattleBase::SetEnemyUnitsDisableState(bool bIsDisabled)
{
	for (auto Unit : EnemyUnits)
	{
		UJRPG_FunctionLibrary::SetActorDisableState(Cast<AActor>(Unit), bIsDisabled);
	}
}

void ABattleBase::SetPlayerUnitsDisableState(bool bIsDisabled)
{
	for (auto Unit : PlayerUnits)
	{
		UJRPG_FunctionLibrary::SetActorDisableState(Cast<AActor>(Unit), bIsDisabled);
	}
}

void ABattleBase::AddActionTime(float ActionTime)
{
	for (auto Unit : PlayerUnits)
	{
		Unit->AddActionTime(ActionTime);
	}
	for (auto Unit : EnemyUnits)
	{
		Unit->AddActionTime(ActionTime);
	}
}

void ABattleBase::Flee()
{
	if (OnBattleOver.IsBound())
	{
		OnBattleOver.Broadcast(EBattleResult::Flee);
	}
}

TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> ABattleBase::GetEnemySpawnData(int SpawnLocationIndex)
{
	TArray<FEnemyUnitSpawnDataList> EnemyList = GetEnemyList();
	TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> Result;
	if (SpawnLocationIndex <= EnemyList.Num() - 1)
	{
		Result = EnemyList[SpawnLocationIndex].EnemyList;
	}
	return Result;
}

void ABattleBase::GetRandomEnemy(TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> EnemySpawnData, TSubclassOf<AEnemyUnitBase> &Unit, int &Level)
{
	float TotalWeight = 0.0f;
	float CurrentWeight = 0.0f;
	for (const auto &Pair : EnemySpawnData)
	{
		TotalWeight += Pair.Value.SpawnChance;
	}
	float RandomWeight = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);
	for (const auto &Pair : EnemySpawnData)
	{
		CurrentWeight += Pair.Value.SpawnChance;
		if (RandomWeight <= CurrentWeight)
		{
			Unit = Pair.Key;
			Level = UKismetMathLibrary::RandomIntegerInRange(Pair.Value.MinLevel, Pair.Value.MaxLevel);
			break;
		}
	}
}

TArray<FEnemyUnitSpawnDataList> ABattleBase::GetEnemyList()
{
	TArray<FEnemyUnitSpawnDataList> EnemyList;
	return EnemyList;
}

FOffLevelBattleData ABattleBase::GetOffLevelBattleData()
{
	FOffLevelBattleData OffLevelBattleData;
	return OffLevelBattleData;
}

bool ABattleBase::TryGetPlayerUnitLevel(TSubclassOf<APlayerUnitBase> UnitClass, int &Level)
{
	bool Result = false;
	if (PlayerUnitLevels.Contains(UnitClass))
	{
		Result = true;
		Level = PlayerUnitLevels[UnitClass];
	}
	{
		UE_LOG(LogTemp, Warning, TEXT("ABattleBase::TryGetPlayerUnitLevel PlayerUnitLevels doesn't contain such unit"));
	}
	return Result;
}