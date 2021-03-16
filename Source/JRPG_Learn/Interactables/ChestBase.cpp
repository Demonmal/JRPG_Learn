// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestBase.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionDetector.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Items/Equipment/EquipmentBase.h"
#include "../Items/UsableItems/UsableItemBase.h"
#include "../Items/MiscItems/MiscItemBase.h"
#include "../UI/Dialogues/ItemObtainDialogue.h"

AChestBase::AChestBase()
{
	PrimaryActorTick.bCanEverTick = 1;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	InteractionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("InteractionActor"));
	InteractionActor->SetupAttachment(RootComponent);

	ChestTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestTop"));
	ChestTop->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AChestBase::BeginPlay()
{
	Super::BeginPlay();
	InteractionDetector = Cast<AInteractionDetector>(InteractionActor->GetChildActor());
	InteractionDetector->OnInteracted.AddUObject(this, &AChestBase::OnInteractedHandler);
	InteractionDetector->OnLoaded.AddUObject(this, &AChestBase::OnLoadedHandler);

	FOnTimelineFloatStatic OpenTimelineUpdate;
	FOnTimelineEventStatic OpenTimelineFinish;
	OpenTimelineUpdate.BindUObject(this, &AChestBase::OpenTimelineUpdate);
	OpenTimelineFinish.BindUObject(this, &AChestBase::OpenTimelineFinished);
	OpenTimeline.AddInterpFloat(OpenTimelineCurve, OpenTimelineUpdate);
	OpenTimeline.SetTimelineFinishedFunc(OpenTimelineFinish);
}

void AChestBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpenTimeline.TickTimeline(DeltaTime);
}

void AChestBase::OnInteractedHandler()
{
	if(bWasInteracted)
		return;
	Open();
	AddUsableItemsToPlayerInventory();
	AddMiscItemsToPlayerInventory();
	AddEquipmentToPlayerInventory();
	AddGoldToPlayerInventory();
	bWasInteracted = true;
}

void AChestBase::AddUsableItemsToPlayerInventory()
{
	InteractionDetector->GetPlayerController()->AddAllUsableItemsToInventory(UsableItems);
}

void AChestBase::AddMiscItemsToPlayerInventory()
{
	InteractionDetector->GetPlayerController()->AddAllMiscItemsToInventory(MiscItems);
}

void AChestBase::AddEquipmentToPlayerInventory()
{
	InteractionDetector->GetPlayerController()->AddAllEquipmentToInventory(Equipment);
}

void AChestBase::AddGoldToPlayerInventory()
{
	InteractionDetector->GetPlayerController()->AddGold(Gold);
}

void AChestBase::Open()
{
	InteractionDetector->GetPlayerController()->SetBlockPlayerUnitState(true);
	OpenTimeline.PlayFromStart();
}

void AChestBase::OpenTimelineUpdate(float Alpha)
{
	FVector TempPosition = FMath::Lerp(ClosedCoverPosition, OpenedCoverPosition, Alpha);
	ChestTop->SetRelativeLocation(TempPosition);
}

void AChestBase::OpenTimelineFinished()
{
	UItemObtainDialogue* ItemObtainDialogue = CreateWidget<UItemObtainDialogue>(GetWorld(), ItemObtainDialogueClass);
	ItemObtainDialogue->UsableItems = UsableItems;
	ItemObtainDialogue->MiscItems = MiscItems;
	ItemObtainDialogue->Equipment = Equipment;
	ItemObtainDialogue->Gold = Gold;
	ItemObtainDialogue->AddToViewport();
	ItemObtainDialogue->OnConfirmed.AddUObject(this, &AChestBase::OnDialogueConfirmed);
}

void AChestBase::OnDialogueConfirmed()
{
	InteractionDetector->GetPlayerController()->SetBlockPlayerUnitState(false);
	InteractionDetector->GetPlayerController()->RefreshInteractions();
	FInputModeGameOnly InputMode;
	InteractionDetector->GetPlayerController()->SetInputMode(InputMode);
}

void AChestBase::OnLoadedHandler(bool bShouldLoadEnemyPawns)
{
	ChestTop->SetRelativeLocation(OpenedCoverPosition);
}