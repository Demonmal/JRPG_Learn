// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ChestBase.generated.h"

class AInteractionDetector;
class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;
class UItemObtainDialogue;

UCLASS()
class JRPG_LEARN_API AChestBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AChestBase();

	virtual void Open();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void AddUsableItemsToPlayerInventory();
	virtual void AddMiscItemsToPlayerInventory();
	virtual void AddEquipmentToPlayerInventory();
	virtual void AddGoldToPlayerInventory();
	
	UFUNCTION()
	void OpenTimelineUpdate(float Alpha);
	UFUNCTION()
	void OpenTimelineFinished();

	virtual void OnInteractedHandler();
	virtual void OnLoadedHandler(bool bShouldLoadEnemyPawns);

	virtual void OnDialogueConfirmed();

	UPROPERTY(VisibleDefaultsOnly)
	class UChildActorComponent* InteractionActor;
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent * ChestTop;

	UPROPERTY()
	AInteractionDetector* InteractionDetector;
	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat *OpenTimelineCurve;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemObtainDialogue> ItemObtainDialogueClass;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItems;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItems;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEquipmentBase>, int> Equipment;
	UPROPERTY(EditAnywhere)
	int Gold;
	UPROPERTY(EditDefaultsOnly)
	FVector ClosedCoverPosition = FVector(.0f, .0f, 20.0f);
	UPROPERTY(EditDefaultsOnly)
	FVector OpenedCoverPosition = FVector(.0f, -30.0f, 20.0f);

	FTimeline OpenTimeline;
	bool bWasInteracted;
};
