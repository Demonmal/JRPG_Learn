// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionDetector.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInteractedSignature)
DECLARE_MULTICAST_DELEGATE(FOnInteractionAppearedSignature)
DECLARE_MULTICAST_DELEGATE(FOnInteractionDisapperedSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadedSignature, bool)

class USphereComponent;

UCLASS()
class JRPG_LEARN_API AInteractionDetector : public AActor
{
	GENERATED_BODY()
	
public:	

	AInteractionDetector();

	FOnInteractedSignature OnInteracted;
	FOnInteractionAppearedSignature OnInteractionAppeared;
	FOnInteractionDisapperedSignature OnInteractionDisappered;
	FOnLoadedSignature OnLoaded;

	void HideInteractionUI();
	void ShowInteractionUI();

	void Load(bool bShouldLoadEnemyPawns);

	void OnInteract();

	FORCEINLINE void SetCanInteractState(bool bState) {bCanInteract = bState;}
	FORCEINLINE void SetPlayerController (AJRPG_PlayerController* Controller) {PlayerController = Controller;}
	FORCEINLINE AJRPG_PlayerController* GetPlayerController () {return PlayerController.Get();}
	FORCEINLINE USphereComponent* GetInteractionSphere () {return InteractionSphere;}
	FORCEINLINE bool GetCanInteract () {return bCanInteract;}
	FORCEINLINE bool IsForceInteraction () {return bForceInteraction;}

protected:

	virtual void BeginPlay() override;

private:

	bool HasUnitGotRequiredSkill();
	bool HasUnitGotEnoughMana();

	void Interact();
	void OnExploreSkillUsed();
	void InteractFailed();
	void NotEnoughManaLogic();
	void ReduceManaOfUnit();
	void UseExploreSkill();

	void OnInteractFailedHandler();
	void OnNotEnoughManaHandler();

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* InteractionSphere;
	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* InteractionWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInfoDialogue> InfoDialogueWidget;
	UPROPERTY(EditDefaultsOnly)
	float InteractionRadius = 100.f;
	UPROPERTY(EditDefaultsOnly)
	FVector InteractionUILocation;
	UPROPERTY(EditDefaultsOnly)
	bool bForceInteraction;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AExploreSkillBase> RequiredSkill;
	UPROPERTY(EditDefaultsOnly)
	FText InteractionText;
	UPROPERTY(EditDefaultsOnly)
	FText InteractionFailText;
	UPROPERTY(EditDefaultsOnly)
	FText NonEnoughManaText;

	UPROPERTY()
	class UInteractionUI* InteractionUI;

	TWeakObjectPtr<class AJRPG_PlayerController> PlayerController;
	TWeakObjectPtr<class ASkillBase> CurrentExploreSkill;
	TWeakObjectPtr<class UJRPG_GameInstance> GameInstance;
	bool bCanInteract = true;
};
