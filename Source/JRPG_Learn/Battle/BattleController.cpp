#include "BattleController.h"
#include "Components/AudioComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "../Controllers/JRPG_GameInstance.h"
#include "../Controllers/JRPG_FunctionLibrary.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Characters/JRPG_CharacterBase.h"
#include "../UI/BattleUI.h"
#include "../UI/Dialogues/VictoryDialogue.h"
#include "../UI/Dialogues/LevelUpDialogue.h"
#include "../UI/Dialogues/DefeatDialogue.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "../Units/EnemyUnits/EnemyUnitBase.h"
#include "../Projectiles/ProjectileBase.h"
#include "../Items/UsableItems/UsableItemBase.h"
#include "../Skills/BattleSkillBase.h"
#include "BattleBase.h"
#include "BattleTransitions/BattleTransitionBase.h"

ABattleController::ABattleController()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	BattleThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleThemeAudio"));
	BattleThemeAudio->SetupAttachment(RootComponent);
	BattleThemeAudio->bAutoActivate = 0;

	VictoryThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("VictoryThemeAudio"));
	VictoryThemeAudio->SetupAttachment(RootComponent);
	VictoryThemeAudio->bAutoActivate = 0;

	ExploreThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ExploreThemeAudio"));
	ExploreThemeAudio->SetupAttachment(RootComponent);
	ExploreThemeAudio->bAutoActivate = 0;
}

void ABattleController::BeginPlay()
{
	Super::BeginPlay();
	PlayExploreTheme();
	UJRPG_GameInstance *GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		GameInstance->SetBattleControllerInstance(this);
		if (GameInstance->GetShouldLoadGame())
		{
			UKismetSystemLibrary::Delay(GetWorld(), 0.1f, LatentActionInfo);
			GameInstance->LoadGame();
		}
	}
}

void ABattleController::PlayExploreTheme()
{
	if (!IsValid(ExploreTheme))
		return;
	ExploreThemeAudio->SetSound(ExploreTheme);
	ExploreThemeAudio->FadeIn(1.0f, 1.0f);
}

void ABattleController::PlayBattleTheme()
{
	if (!IsValid(BattleTheme))
		return;
	BattleThemeAudio->SetSound(BattleTheme);
	BattleThemeAudio->Play();
}

void ABattleController::InitBattle(ABattleBase *Battle, AJRPG_PlayerController *Controller)
{
	CurrentBattle = TWeakObjectPtr<ABattleBase>(Battle);
	PlayerController = TWeakObjectPtr<AJRPG_PlayerController>(Controller);
	ExploreCharacter = TWeakObjectPtr<APawn>(Controller->GetExploreCharacter());
	if (GetBattleLevelName().IsNone())
	{
		StartTransition();
		bIsBattleOver = false;
		CurrentBattle->OnBattleOver.AddUObject(this, &ABattleController::OnBattleOverHandler);
	}
	else
	{
		ChangeMapForBattle();
	}
}

void ABattleController::OnBattleOverHandler(EBattleResult BattleResult_l)
{
	CurrentBattle->OnBattleOver.RemoveAll(this);
	bIsBattleOver = true;
	BattleResult = BattleResult_l;
}

void ABattleController::SwitchToBattleMode()
{
	ExploreCharacter->GetMovementComponent()->StopMovementImmediately();
	PlayerController->ChangeGameState(EGameState::Battle);
	PlayerController->UnPossess();
	PlayerController->RemovePartyMemberCharacters();
	ExploreThemeAudio->FadeOut(1.0f, 0.0f);
	AJRPG_CharacterBase *Character = PlayerController->GetExploreCharacter();
	if (IsValid(Character))
	{
		Character->RemoveAllEquipmentMesh();
	}
}

void ABattleController::SwitchToExploreMode()
{
	CurrentBattle->RemoveBattle();
	PlayerController->Possess(ExploreCharacter.Get());
	PlayerController->OnBattleOver();
	PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 2.0f, FLinearColor::Black, false, true);
	VictoryThemeAudio->FadeOut(1.0f, 0.0f);
	PlayExploreTheme();
}

void ABattleController::StartBattle()
{
	IncreaseTurnCount();
	PlayBattleTheme();
	CurrentBattle->InitUnits();
	if (SwitchToStaticCamera())
	{
		BattleUI = CreateWidget<UBattleUI>(GetWorld(), BattleUIClass);
		BattleUI->AddToViewport();
		BattleUI->ShowBattleUI();
		SetReadyUnits();
	}
	else
	{
		AdjustCamera(CurrentBattle->GetPlayerUnits()[0], Cast<AUnitBase>(CurrentBattle->GetEnemyUnits()[0]), true);
	}
}

void ABattleController::IncreaseTurnCount()
{
	CurrentTurn++;
	if (OnTurnChanged.IsBound())
	{
		OnTurnChanged.Broadcast(CurrentTurn);
	}
}

void ABattleController::SetReadyUnits()
{
	AttackUnitIndex = 0;
	switch (TurnType)
	{
	case ETurnType::ActionTime:
		float ClosestActionTime;
		ReadyToAttackUnits = GetReadyUnitsBasedOnAT(ClosestActionTime);
		if (OnActionTimeAdded.IsBound())
		{
			OnActionTimeAdded.Broadcast(ClosestActionTime);
		}
		CurrentBattle->AddActionTime(ClosestActionTime);
		UKismetSystemLibrary::Delay(GetWorld(), ClosestActionTime, LatentActionInfo);
		break;
	case ETurnType::Speed:
		IncreaseTurnCount();
		ReadyToAttackUnits = GetReadyUnitsBasedOnSpeed();
		UKismetSystemLibrary::Delay(GetWorld(), 2.0f, LatentActionInfo);
		break;
	}
	OnUnitsTurnStarted();
}

void ABattleController::OnUnitsTurnStarted()
{
	CurrentAttackingUnit = ReadyToAttackUnits[AttackUnitIndex];
	if (CurrentAttackingUnit.IsValid())
	{
		CurrentAttackingUnit->OnTurnEnded.AddUObject(this, &ABattleController::OnTurnEndedHandler);
		if (!CurrentAttackingUnit->IsUnitDead())
		{
			BattleUI->UpdateTurnOrderList(CurrentAttackingUnit.Get());
		}
		if (CurrentAttackingUnit->IsStuned())
		{
			CurrentAttackingUnit->EndTurn();
		}
		else
		{
			CurrentAttackingUnit->StartTurn();
			APlayerUnitBase *PlayerUnit = Cast<APlayerUnitBase>(CurrentAttackingUnit);
			if (IsValid(PlayerUnit))
			{
				BattleUI->ShowUnitBattleDetails();
			}
			else
			{
				BattleUI->HideUnitBattleDetails();
			}
		}
	}
	else
	{
		OnTurnEndedHandler();
	}
}

void ABattleController::OnTurnEndedHandler()
{
	ReadyToAttackUnits[AttackUnitIndex]->OnTurnEnded.RemoveAll(this);
	CurrentAttackingUnit = nullptr;
	BattleUI->HideUnitBattleDetails();
	OnUnitsTurnEnded();
}

void ABattleController::OnUnitsTurnEnded()
{
	if (bIsBattleOver)
	{
		OnBattleOver();
	}
	else
	{
		if (++AttackUnitIndex > ReadyToAttackUnits.Num() - 1)
		{
			SetReadyUnits();
		}
		else
		{
			OnUnitsTurnStarted();
		}
	}
}

void ABattleController::OnBattleOver()
{
	BattleThemeAudio->FadeOut(1.0f, 0.0f);
	switch (BattleResult)
	{
	case EBattleResult::PlayerWon:
		PlayerWon();
		break;
	case EBattleResult::EnemyWon:
		EnemyWon();
		break;
	case EBattleResult::Flee:
		TArray<TSubclassOf<APlayerUnitBase>> DeadPlayerUnits;
		CurrentBattle->GetDeadPlayerUnits().GetKeys(DeadPlayerUnits);
		PlayerController->UpdatePlayerUnitsData(0, CurrentBattle->GetPlayerUnits(), DeadPlayerUnits);
		SwitchToExploreMode();
		break;
	}
}

void ABattleController::PlayerWon()
{
	TArray<TSubclassOf<APlayerUnitBase>> DeadPlayerUnits;
	CurrentBattle->GetDeadPlayerUnits().GetKeys(DeadPlayerUnits);
	PlayerController->UpdatePlayerUnitsData(CurrentBattle->GetTotalExpReward(), CurrentBattle->GetPlayerUnits(), DeadPlayerUnits);
	PlayerController->AddGold(CurrentBattle->GetTotalGoldReward());
	AddUsableItemsToPlayerInventory();
	AddMiscItemsToPlayerInventory();
	AddEquipmentToPlayerInventory();
	UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
	UVictoryDialogue *VictoryDialogue = CreateWidget<UVictoryDialogue>(GetWorld(), VictoryDialogueClass);
	VictoryDialogue->ExpReward = CurrentBattle->GetTotalExpReward();
	VictoryDialogue->GoldReward = CurrentBattle->GetTotalGoldReward();
	VictoryDialogue->UsableItemDrops = CurrentBattle->GetUsableItemDrops();
	VictoryDialogue->MiscItemDrops = CurrentBattle->GetMiscItemDrops();
	VictoryDialogue->EquipmentDrops = CurrentBattle->GetEquipmentDrops();
	VictoryDialogue->AddToViewport();
	VictoryDialogue->OnContinueClicked.AddUObject(this, &ABattleController::OnContinueClickedHandler);
	if (IsValid(VictoryTheme))
	{
		VictoryThemeAudio->SetSound(VictoryTheme);
		VictoryThemeAudio->Play();
	}
}

void ABattleController::OnContinueClickedHandler()
{
	LeveledUpUnits = GetLeveledUpUnits();
	if (LeveledUpUnits.Num() > 0)
	{
		LeveledUpUnitIndex = 0;
		ShowLeveledUpUnit();
	}
	else
	{
		SwitchToExploreMode();
	}
}

TMap<TSubclassOf<APlayerUnitBase>, int> ABattleController::GetLeveledUpUnits()
{
	TMap<TSubclassOf<APlayerUnitBase>, int> CurrentLeveledUpUnits;
	TArray<TSubclassOf<APlayerUnitBase>> PartyMembers = PlayerController->GetPartyMembers();
	FPlayerUnitData PlayerUnitData;
	FUnitStats UnitStats;
	for (const auto &PartyMember : PartyMembers)
	{
		if (PlayerController->TryGetUnitDataByPlayer(PartyMember, PlayerUnitData))
		{
			int Level;
			UnitStats = UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(PartyMember, PlayerUnitData, Level);
			int UnitLevel;
			if (CurrentBattle->TryGetPlayerUnitLevel(PartyMember, UnitLevel))
			{
				if (Level - UnitLevel > 0)
				{
					CurrentLeveledUpUnits.Add(PartyMember, Level - UnitLevel);
					PlayerUnitData.CurrentHP = UnitStats.MaxHP;
					PlayerUnitData.CurrentMP = UnitStats.MaxMP;
				}
			}
		}
	}
	return CurrentLeveledUpUnits;
}

void ABattleController::ShowLeveledUpUnit()
{
	TArray<APlayerUnitBase *> PlayerUnits = CurrentBattle->GetPlayerUnits();
	TArray<TSubclassOf<APlayerUnitBase>> LeveledUpUnitClasses;
	LeveledUpUnits.GetKeys(LeveledUpUnitClasses);
	TSubclassOf<APlayerUnitBase> LeveledUpUnitClass = LeveledUpUnitClasses[LeveledUpUnitIndex];
	for (auto Unit : PlayerUnits)
	{
		if (LeveledUpUnitClass == Unit->GetClass())
		{
			Unit->SwitchToFrontCamera(false);
			ULevelUpDialogue *LevelUpDialogue = CreateWidget<ULevelUpDialogue>(GetWorld(), LevelUpDialogueClass);
			LevelUpDialogue->PlayerUnit = LeveledUpUnitClass;
			LevelUpDialogue->LevelUpAmount = LeveledUpUnits[LeveledUpUnitClass];
			LevelUpDialogue->BattleController = this;
			FPlayerUnitData PlayerUnitData;
			if (PlayerController->TryGetUnitDataByPlayer(LeveledUpUnitClass, PlayerUnitData))
			{
				LevelUpDialogue->PlayerUnitData = PlayerUnitData;
			}
			LevelUpDialogue->AddToViewport();
			LevelUpDialogue->OnContinueClicked.AddUObject(this, &ABattleController::OnLevelUpContinueClickedHandler);
			break;
		}
	}
}

void ABattleController::OnLevelUpContinueClickedHandler()
{
	LeveledUpUnitIndex++;
	if (LeveledUpUnitIndex < LeveledUpUnits.Num())
	{
		ShowLeveledUpUnit();
	}
	else
	{
		SwitchToExploreMode();
	}
}

void ABattleController::EnemyWon()
{
	UKismetSystemLibrary::Delay(GetWorld(), 2.0f, LatentActionInfo);
	UDefeatDialogue *DefeatDialogue = CreateWidget<UDefeatDialogue>(GetWorld(), DefeatDialogueClass);
	DefeatDialogue->AddToViewport();
}

void ABattleController::AddUsableItemsToPlayerInventory()
{
	PlayerController->AddAllUsableItemsToInventory(CurrentBattle->GetUsableItemDrops());
}

void ABattleController::AddMiscItemsToPlayerInventory()
{
	PlayerController->AddAllMiscItemsToInventory(CurrentBattle->GetMiscItemDrops());
}

void ABattleController::AddEquipmentToPlayerInventory()
{
	PlayerController->AddAllEquipmentToInventory(CurrentBattle->GetEquipmentDrops());
}

TArray<AUnitBase *> ABattleController::GetReadyUnitsBasedOnAT(float &ClosestActionTime)
{
	TArray<AUnitBase *> AllUnits;
	TArray<AUnitBase *> CurrentReadyToAttackUnits;
	ClosestActionTime = 10000000.0f;
	AllUnits.Append(CurrentBattle->GetPlayerUnits());
	AllUnits.Append(CurrentBattle->GetEnemyUnits());
	float RestOfActionTime;
	for (auto Unit : AllUnits)
	{
		RestOfActionTime = Unit->GetActionTime() - Unit->GetCurrentActionTime();
		if (FMath::IsNearlyEqual(RestOfActionTime, ClosestActionTime))
		{
			CurrentReadyToAttackUnits.Add(Unit);
		}
		else if (RestOfActionTime < ClosestActionTime)
		{
			ClosestActionTime = RestOfActionTime;
			CurrentReadyToAttackUnits.Empty();
			CurrentReadyToAttackUnits.Add(Unit);
		}
	}
	return AllUnits;
}

TArray<AUnitBase *> ABattleController::GetReadyUnitsBasedOnSpeed()
{
	TArray<AUnitBase *> AllUnits;
	TArray<AUnitBase *> CurrentReadyToAttackUnits;
	AUnitBase *HighestSpeedUnit;
	AllUnits.Append(CurrentBattle->GetPlayerUnits());
	AllUnits.Append(CurrentBattle->GetEnemyUnits());
	while (AllUnits.Num() != 0)
	{
		HighestSpeedUnit = GetUnitWithHighestSpeed(AllUnits);
		CurrentReadyToAttackUnits.Add(HighestSpeedUnit);
		AllUnits.Remove(HighestSpeedUnit);
	}
	return CurrentReadyToAttackUnits;
}

AUnitBase *ABattleController::GetUnitWithHighestSpeed(TArray<AUnitBase *> Units)
{
	AUnitBase *HighestSpeedUnit = Units[0];
	int CurrentHighestSpeed = 0;
	int Speed;
	for (auto Unit : Units)
	{
		Speed = Unit->GetSpeed();
		if (Speed > CurrentHighestSpeed)
		{
			CurrentHighestSpeed = Speed;
			HighestSpeedUnit = Unit;
		}
	}
	return HighestSpeedUnit;
}

void ABattleController::UpdateUnitBattleDetails(AUnitBase *AttackingUnit, AUnitBase *TargetUnit)
{
	BattleUI->UpdateAttackingUnitBattleDetails(AttackingUnit);
	BattleUI->UpdateTargetUnitBattleDetails(TargetUnit);
}

void ABattleController::AdjustCamera(APlayerUnitBase *PlayerUnit, AUnitBase *TargetUnit, bool bIsInstant)
{
	if (CurrentBattle->IsDynamicCamera())
	{
		PlayerUnit->AdjustBackCamera(TargetUnit, bIsInstant);
	}
}

void ABattleController::Attack(AUnitBase *TargetUnit)
{
	CurrentTargetUnit = TargetUnit;
	if (CurrentAttackingUnit->IsRangeUnit())
	{
		RangeAttack();
	}
	else
	{
		if (bShouldUnitsMoveToTarget)
		{
			MoveToTarget(CurrentTargetUnit->GetAttackerLocation()->GetComponentLocation(), &ABattleController::OnUnitMovedToTargetHandler, true);
		}
		else
		{
			MeleeAttack();
		}
	}
}

void ABattleController::OnUnitMovedToTargetHandler()
{
	MeleeAttack();
}

void ABattleController::RangeAttack()
{
	bRangeAttackAnimHasEnded = false;
	bHasProjectileHit = false;
	UAnimMontage *AttackAnimMontage = CurrentAttackingUnit->GetAttackAnimMontage();
	FScriptDelegate MontageNotifyDelegate;
	MontageNotifyDelegate.BindUFunction(this, "OnRangeAttackAnimMontageNotified");
	CurrentAttackingUnit->GetAnimInstance()->OnPlayMontageNotifyBegin.Add(MontageNotifyDelegate);
	CurrentAttackingUnit->PlayAnimMontage(AttackAnimMontage);
	UKismetSystemLibrary::Delay(GetWorld(), AttackAnimMontage->SequenceLength, LatentActionInfo);
	bRangeAttackAnimHasEnded = true;
	if (bHasProjectileHit)
	{
		UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
		CurrentAttackingUnit->EndTurn();
	}
}

void ABattleController::MeleeAttack()
{
	UAnimMontage *AttackAnimMontage = CurrentAttackingUnit->GetAttackAnimMontage();
	FScriptDelegate MontageNotifyDelegate;
	MontageNotifyDelegate.BindUFunction(this, "OnMeleeAttackAnimMontageNotified");
	CurrentAttackingUnit->GetAnimInstance()->OnPlayMontageNotifyBegin.Add(MontageNotifyDelegate);
	CurrentAttackingUnit->PlayAnimMontage(AttackAnimMontage);
	UKismetSystemLibrary::Delay(GetWorld(), AttackAnimMontage->SequenceLength, LatentActionInfo);
	if (bShouldUnitsMoveToTarget)
	{
		MoveToTarget(CurrentAttackingUnit->GetInitialLocation(), &ABattleController::OnUnitMovedBackHandler, false);
	}
	else
	{
		CurrentAttackingUnit->EndTurn();
	}
}

void ABattleController::OnUnitMovedBackHandler()
{
	CurrentAttackingUnit->EndTurn();
}

void ABattleController::OnMeleeAttackAnimMontageNotified(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload)
{
	if (NotifyName == AttackNotifyName)
	{
		DealDamage(0, 1.0f);
	}
}

void ABattleController::OnRangeAttackAnimMontageNotified(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload)
{
	if (NotifyName == AttackNotifyName)
	{
		AProjectileBase *Projectile = GetWorld()->SpawnActor<AProjectileBase>(CurrentAttackingUnit->GetProjectileClass(), CurrentAttackingUnit->GetProjectileSpawnTransform());
		Projectile->SetTargetUnit(CurrentTargetUnit.Get());
		Projectile->OnProjectileHit.AddUObject(this, &ABattleController::OnProjectileHitHandler);
		Projectile->MoveProjectileToTarget();
	}
}

void ABattleController::OnProjectileHitHandler()
{
	DealDamage(0, 1.0f);
	bHasProjectileHit = true;
	if (bRangeAttackAnimHasEnded)
	{
		UKismetSystemLibrary::Delay(GetWorld(), 1.0f, LatentActionInfo);
		CurrentAttackingUnit->EndTurn();
	}
}

void ABattleController::ShowSkills(APlayerUnitBase *PlayerUnit)
{
	BattleUI->ShowSkills(PlayerUnit);
	BattleUI->OnSkillSelected.AddUObject(this, &ABattleController::OnSkillSelectedHandler);
	BattleUI->OnSkillUIClosed.AddUObject(this, &ABattleController::OnSkillUIClosed);
	BattleUI->OnSkillUsed.AddUObject(this, &ABattleController::OnSkillUsedUIHandler);
	BattleUI->OnSkillUseCancelled.AddUObject(this, &ABattleController::OnSkillUseCancelled);
}

void ABattleController::OnSkillSelectedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill)
{
	if(IsUnitHasEnoughMP(SelectedSkill))
	{
		BattleUI->OnSkillSelected.RemoveAll(this);
		BattleUI->OnUnitHasEnoughMP();
		Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->ChangeCameraForSkill(SelectedSkill);
		FVector SpawnLocation{10000.0f, 10000.0f, 10000.0f};
		FRotator SpawnRotation;
		CurrentSkill = GetWorld()->SpawnActor<ABattleSkillBase>(SelectedSkill, SpawnLocation, SpawnRotation);
		CurrentSkill->SetBattleController(this);
		CurrentSkill->SetPlayerController(PlayerController.Get());
	}
}

void ABattleController::OnSkillUIClosed()
{
	BattleUI->OnSkillUIClosed.RemoveAll(this);
	BattleUI->HideSkills();
	BattleUI->HideSkillActionButtons();
	CurrentAttackingUnit->StartTurn();
}

void ABattleController::OnSkillUsedUIHandler()
{
	BattleUI->OnSkillUsed.RemoveAll(this);
	Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->RemoveUI();
	UseSkill(CurrentAttackingUnit->GetCurrentTarget());
}

void ABattleController::OnSkillUseCancelled()
{
	BattleUI->OnSkillUseCancelled.RemoveAll(this);
	Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->RemoveUI();
	BattleUI->HideSkillActionButtons();
	CurrentAttackingUnit->StartTurn();
	CurrentUsableItem->Destroy();
	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
}

void ABattleController::UseSkill(AUnitBase *TargetUnit)
{
	CurrentTargetUnit = TargetUnit;
	CurrentSkill->OnSkillUsed.AddUObject(this, &ABattleController::OnSkillUsedHandler);
	CurrentAttackingUnit->UseMP(CurrentSkill->GetManaRequired());
	CurrentSkill->UseSkill();
	BattleUI->HideSkillActionButtons();
}

void ABattleController::EnemyUseSkill()
{
	CurrentSkill->OnSkillUsed.AddUObject(this, &ABattleController::OnSkillUsedHandler);
	CurrentSkill->UseSkill();
}

void ABattleController::EnemyPrepareForSkill(TSubclassOf<ABattleSkillBase> BattleSkill)
{
	FVector SpawnLocation{10000.0f, 10000.0f, 10000.0f};
	FRotator SpawnRotation;
	CurrentSkill = GetWorld()->SpawnActor<ABattleSkillBase>(BattleSkill, SpawnLocation, SpawnRotation);
	CurrentTargetUnit = CurrentSkill->GetTargetUnitForEnemy();
	Cast<AEnemyUnitBase>(CurrentAttackingUnit.Get())->AdjustCameraForSkill(CurrentTargetUnit.Get());
}

void ABattleController::OnSkillUsedHandler()
{
	CurrentSkill->Destroy();
	CurrentAttackingUnit->EndTurn();
}

bool ABattleController::IsUnitHasEnoughMP(TSubclassOf<ABattleSkillBase> Skill)
{
	ABattleSkillBase *SkillDefault = Cast<ABattleSkillBase>(Skill->GetDefaultObject());
	return CurrentAttackingUnit->GetCurrentMP() >= SkillDefault->GetManaRequired();
}

void ABattleController::ShowItemInventory()
{
	BattleUI->ShowUsableItemInventory();
	BattleUI->OnItemSelected.AddUObject(this, &ABattleController::OnItemSelectedHandler);
	BattleUI->OnItemUsed.AddUObject(this, &ABattleController::OnItemUsedHandler);
	BattleUI->OnItemUseCancelled.AddUObject(this, &ABattleController::OnItemUseCancelledHandler);
	BattleUI->OnItemInventoryClosed.AddUObject(this, &ABattleController::OnItemInventoryClosedHandler);
}

void ABattleController::OnItemSelectedHandler(TSubclassOf<AItemBase> SelectedItem)
{
	BattleUI->OnItemSelected.RemoveAll(this);
	TSubclassOf<AUsableItemBase> UsableItemClass(SelectedItem);
	if (UKismetSystemLibrary::IsValidClass(UsableItemClass))
	{
		FVector SpawnLocation{10000.0f, 10000.0f, 10000.0f};
		FRotator SpawnRotation;
		CurrentUsableItem = GetWorld()->SpawnActor<AUsableItemBase>(UsableItemClass, SpawnLocation, SpawnRotation);
		switch (CurrentUsableItem->GetUnitFilter())
		{
		case EUnitFilter::All:
		case EUnitFilter::Alive:
			Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->ChangeTargetForUseItem();
			break;
		case EUnitFilter::Dead:
			UKismetSystemLibrary::Delay(GetWorld(), .1f, LatentActionInfo);

			break;
		}
	}
}

void ABattleController::OnItemUseCancelledHandler()
{
	BattleUI->OnItemUseCancelled.RemoveAll(this);
	Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->RemoveUI();
	BattleUI->HideItemsActionButtons();
	CurrentAttackingUnit->StartTurn();
	CurrentUsableItem->Destroy();
	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
}

void ABattleController::OnItemInventoryClosedHandler()
{
	BattleUI->OnItemInventoryClosed.RemoveAll(this);
	BattleUI->HideUsableItemInventory();
	BattleUI->HideItemsActionButtons();
	CurrentAttackingUnit->StartTurn();
}

void ABattleController::OnItemUsedHandler()
{
	BattleUI->OnItemUsed.RemoveAll(this);
	Cast<APlayerUnitBase>(CurrentAttackingUnit.Get())->RemoveUI();
	TSubclassOf<APlayerUnitBase> UnitClass;
	UseItem(CurrentAttackingUnit->GetCurrentTarget(), UnitClass);
}

void ABattleController::UseItem(AUnitBase *TargetUnit, TSubclassOf<APlayerUnitBase> UnitClass)
{
	CurrentUsableItem->OnItemUsed.AddUObject(this, &ABattleController::OnItemUsed);
	CurrentUsableItem->Use(this, PlayerController.Get(), true, TargetUnit, UnitClass);
}

void ABattleController::OnItemUsed()
{
	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	CurrentAttackingUnit->EndTurn();
	CurrentUsableItem->Destroy();
}

void ABattleController::Flee(APlayerUnitBase *PlayerUnit)
{
	if (CurrentBattle->GetFleeChance() >= UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f))
	{
		CurrentBattle->Flee();
		BattleUI->HideUnitBattleDetails();
		OnBattleOver();
	}
	else
	{
		PlayerUnit->EndTurn();
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
	}
}

void ABattleController::StartTransition()
{
}

void ABattleController::ChangeMapForBattle()
{
}

bool ABattleController::DealDamage(int PureDamage, float DamageMultiplier)
{
	bool bIsHit = false;
	return bIsHit;
}

TSubclassOf<ABattleTransitionBase> ABattleController::GetBattleTransitionClass()
{
	TSubclassOf<ABattleTransitionBase> OverrideBattleTransitionClass = CurrentBattle->GetOverrideBattleTransitionClass();
	bool bIsOverrideTransitionValid = UKismetSystemLibrary::IsValidClass(OverrideBattleTransitionClass);
	return bIsOverrideTransitionValid ? OverrideBattleTransitionClass : BattleTransition;
}

FName ABattleController::GetBattleLevelName()
{
	FName Result;
	return Result;
}

bool ABattleController::SwitchToStaticCamera()
{
	bool Result = false;
	AActor *StaticCamera = CurrentBattle->GetStaticCamera();
	if (IsValid(StaticCamera))
	{
		PlayerController->SetViewTargetWithBlend(StaticCamera, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		Result = true;
	}
	return Result;
}

void ABattleController::MoveToTarget(FVector TargetLocation, void (ABattleController::*InFunc)(), bool bIsForward)
{
	CurrentAttackingUnit->OnMovedToTarget.AddUObject(this, InFunc);
	CurrentAttackingUnit->MoveToTarget(TargetLocation, bIsForward);
}