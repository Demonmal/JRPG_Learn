// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionDetector.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Controllers/JRPG_FunctionLibrary.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Controllers/JRPG_GameInstance.h"
#include "../Controllers/AudioPlayerController.h"
#include "../UI/InteractionUI.h"
#include "../UI/Dialogues/InfoDialogue.h"
#include "../Characters/JRPG_CharacterBase.h"
#include "../Skills/BattleSkillBase.h"
#include "../Skills/ExploreSkillBase.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "NavigationSystem/Public/NavAreas/NavArea_Obstacle.h"

AInteractionDetector::AInteractionDetector()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->AreaClass = FNavigationSystem::GetDefaultObstacleArea();
	InteractionSphere->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetDrawAtDesiredSize(true);
	InteractionWidget->SetupAttachment(RootComponent);
}

void AInteractionDetector::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	InteractionSphere->SetSphereRadius(InteractionRadius);
	InteractionWidget->SetRelativeLocation(InteractionUILocation);
	InteractionUI = Cast<UInteractionUI>(InteractionWidget->GetUserWidgetObject());
	InteractionUI->HideInteractionUI();
	InteractionUI->Init(InteractionText);
	InteractionUI->OnInteracted.AddUObject(this, &AInteractionDetector::OnInteract);
}

void AInteractionDetector::OnInteract()
{
	GameInstance->AudioPlayerController(GetWorld())->PlaySoundByTag("Confirm");
	HideInteractionUI();
	if (UKismetSystemLibrary::IsValidClass(RequiredSkill))
	{
		if(HasUnitGotRequiredSkill())
		{
			if(HasUnitGotEnoughMana())
			{
				ReduceManaOfUnit();
				UseExploreSkill();
			}
			else
			{
				NotEnoughManaLogic();
			}
		}
		else
		{
			InteractFailed();
		}
	}
	else
	{
		Interact();
	}
}

void AInteractionDetector::HideInteractionUI()
{
	InteractionUI->HideInteractionUI();
	if (OnInteractionDisappered.IsBound())
	{
		OnInteractionDisappered.Broadcast();
	}
}

void AInteractionDetector::ShowInteractionUI()
{
	InteractionUI->ShowInteractionUI();
	if (OnInteractionAppeared.IsBound())
	{
		OnInteractionAppeared.Broadcast();
	}
}

void AInteractionDetector::Interact()
{
	bCanInteract = false;
	if (OnInteracted.IsBound())
	{
		OnInteracted.Broadcast();
	}
	GameInstance->AddInteractions(this);
}

bool AInteractionDetector::HasUnitGotRequiredSkill()
{
	bool Result = false;
	FPlayerUnitData UnitData;
	if (PlayerController->TryGetUnitDataByPlayer(PlayerController->GetExploreCharacter()->GetPlayerUnit(), UnitData))
	{
		TArray<TSubclassOf<ABattleSkillBase>> EquipmentBattleSkills;
		TArray<TSubclassOf<AExploreSkillBase>> EquipmentExploreSkills;
		UJRPG_FunctionLibrary::GetEquipmentSkills(UnitData, EquipmentBattleSkills, EquipmentExploreSkills);
		if(EquipmentExploreSkills.Contains(RequiredSkill))
		{
			Result = true;
		}
		else
		{
			APlayerUnitBase* UnitDefault = Cast<APlayerUnitBase>(PlayerController->GetExploreCharacter()->GetPlayerUnit()->GetDefaultObject());
			TMap<TSubclassOf<AExploreSkillBase>, int> ExploreSkills = UnitDefault->GetExploreSkills();
			if(ExploreSkills.Contains(RequiredSkill))
			{
				int SkillLevel = ExploreSkills[RequiredSkill];
				int Level = UJRPG_FunctionLibrary::GetLevel(UnitData.Exp, UnitDefault->GetExpExponent(), UnitDefault->GetMaxLevel());
				if(SkillLevel<=Level)
				{
					Result = true;
				}
			}
		}
	}
	return Result;
}

bool AInteractionDetector::HasUnitGotEnoughMana()
{
	bool Result = false;
	FPlayerUnitData UnitData;
	if (PlayerController->TryGetUnitDataByPlayer(PlayerController->GetExploreCharacter()->GetPlayerUnit(), UnitData))
	{
		ASkillBase* SkillDefault = Cast<ASkillBase>(RequiredSkill->GetDefaultObject());
		if(UnitData.CurrentMP >= SkillDefault->GetManaRequired())
		{
			Result = true;
		}
	}
	return Result;
}

void AInteractionDetector::InteractFailed()
{
	UInfoDialogue* InfoDialogue = CreateWidget<UInfoDialogue>(GetWorld(), InfoDialogueWidget);
	InfoDialogue->DialogueText = InteractionFailText;
	InfoDialogue->AddToViewport();
	InfoDialogue->OnConfirmed.AddUObject(this, &AInteractionDetector::OnInteractFailedHandler);
}

void AInteractionDetector::NotEnoughManaLogic()
{
	UInfoDialogue* InfoDialogue = CreateWidget<UInfoDialogue>(GetWorld(), InfoDialogueWidget);
	InfoDialogue->DialogueText = NonEnoughManaText;
	InfoDialogue->AddToViewport();
	InfoDialogue->OnConfirmed.AddUObject(this, &AInteractionDetector::OnNotEnoughManaHandler);
}

void AInteractionDetector::OnInteractFailedHandler()
{
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->RefreshInteractions();
}

void AInteractionDetector::OnNotEnoughManaHandler()
{
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->RefreshInteractions();
}

void AInteractionDetector::ReduceManaOfUnit()
{
	FPlayerUnitData UnitData;
	if (PlayerController->TryGetUnitDataByPlayer(PlayerController->GetExploreCharacter()->GetPlayerUnit(), UnitData))
	{
		ASkillBase* SkillDefault = Cast<ASkillBase>(RequiredSkill->GetDefaultObject());
		UnitData.CurrentMP -= SkillDefault->GetManaRequired();
	}
}

void AInteractionDetector::UseExploreSkill()
{
	CurrentExploreSkill = GetWorld()->SpawnActor<ASkillBase>(RequiredSkill);
	CurrentExploreSkill->SetPlayerController(PlayerController.Get());
	CurrentExploreSkill->OnSkillUsed.AddUObject(this, &AInteractionDetector::OnExploreSkillUsed);
	PlayerController->SetBlockPlayerUnitState(true);
	CurrentExploreSkill->UseSkill();
}

void AInteractionDetector::OnExploreSkillUsed()
{
	CurrentExploreSkill->OnSkillUsed.RemoveAll(this);
	CurrentExploreSkill->Destroy();
	Interact();
}

void AInteractionDetector::Load(bool bShouldLoadEnemyPawns)
{
	SetCanInteractState(false);
	if (OnLoaded.IsBound())
	{
		OnLoaded.Broadcast(bShouldLoadEnemyPawns);
	}
}