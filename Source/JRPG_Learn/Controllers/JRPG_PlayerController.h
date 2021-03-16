// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Data/DataTypes.h"
#include "../Data/Enums.h"
#include "JRPG_PlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState)
DECLARE_MULTICAST_DELEGATE(FOnPossessedCharacterSignature)

class APlayerUnitBase;
class AItemBase;
class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;
class AJRPG_CharacterBase;
class AQuestBase;

UCLASS()
class JRPG_LEARN_API AJRPG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn *InPawn) override;

	FOnGameStateChangedSignature OnGameStateChanged;
	FOnPossessedCharacterSignature OnPossessedCharacter;

	void SetUnitsStartingStats();
	void SetPartyMembers();
	void SetExploreCharacter();
	void ShowPartyLeader();
	void ShowAllPartyMembers();
	void RemovePartyMemberCharacters();
	void ShowAllEquipmentMeshes(AJRPG_CharacterBase *);
	EQuestStatus GetQuestStatus(TSubclassOf<AQuestBase>);
	bool IsQuestAvailable(TSubclassOf<AQuestBase>);
	FTransform GetExploreCharacterTransform();
	void BindOverlapEvents();
	void UnbindOverlapEvents();
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                                                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	void AddAllUsableItemsToInventory(TMap<TSubclassOf<AUsableItemBase>, int>);
	void AddAllMiscItemsToInventory(TMap<TSubclassOf<AMiscItemBase>, int>);
	void AddAllEquipmentToInventory(TMap<TSubclassOf<AEquipmentBase>, int>);
	void AddGold(float Amount);

	void OnUsableItemUsed(TSubclassOf<AUsableItemBase>, int);

	void RefreshInteractions();

	FORCEINLINE void SetBlockPlayerUnitState(bool bState) {bIsInputBlocked = bState;}

	bool TryGetUnitDataByPlayer(TSubclassOf<APlayerUnitBase>, FPlayerUnitData &);

	FORCEINLINE AJRPG_CharacterBase* GetExploreCharacter() {return ExploreCharacter.Get();}
	FORCEINLINE bool IsQuestCompleted(TSubclassOf<AQuestBase> Quest) {return CompletedQuests.Contains(Quest);}

protected:
	virtual void SetupInputComponent() override;

private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void TurnRate(float AxisValue);
	void LookUpRate(float AxisValue);
	void PossessNextCharacter();
	void PossessPreviousCharacter();
	void OnInteractedHandler();

	void AddUsableItemToInventory(TSubclassOf<AUsableItemBase>, int);
	void AddMiscItemToInventory(TSubclassOf<AMiscItemBase>, int);
	void AddEquipmentToInventory(TSubclassOf<AEquipmentBase>, int);
	int GetItemAmount(TSubclassOf<AItemBase>);

	void UpdateItemQuests(TSubclassOf<AItemBase>);
	void UpdateQuest(TSubclassOf<AQuestBase>, int, bool, bool);

	void OnNewCharacterPossessed();
	void ClearInteractable();

	void SetPartyLeaderForAICharacters();

	void DetectInteraction(AActor *, UPrimitiveComponent *);

	UPROPERTY(EditDefaultsOnly)
	float BaseTurnRate = 45.f;
	UPROPERTY(EditDefaultsOnly)
	float BaseLookUpRate = 45.f;
	UPROPERTY(EditDefaultsOnly)
	int PartySize = 3;
	UPROPERTY(EditDefaultsOnly)
	int Gold = 10;
	UPROPERTY(EditDefaultsOnly)
	EExploreCharacterMode ExploreCharacterMode;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<APlayerUnitBase>, FPlayerUnitData> PlayerUnits;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItems;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItems;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AEquipmentBase>, int> Equipments;

	UPROPERTY()
	TArray<TSubclassOf<APlayerUnitBase>> PartyMembers;
	UPROPERTY()
	TArray<AJRPG_CharacterBase *> PartyMemberCharacters;
	UPROPERTY()
	TMap<TSubclassOf<AQuestBase>, int> ActiveQuests;
	UPROPERTY()
	TArray<TSubclassOf<AQuestBase>> CompletedQuests;

	TWeakObjectPtr<AJRPG_CharacterBase> CachedCharacter;
	TWeakObjectPtr<AJRPG_CharacterBase> ExploreCharacter;
	TWeakObjectPtr<class AInteractionDetector> Interactable;

	float BlendTimeToNewCharacter = 0.5f;
	float MinPartyMemberSpawnOffset = -50.f;
	float MaxPartyMemberSpawnOffset = -100.f;

	bool bIsInputBlocked;
};
