// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/Enums.h"
#include "JRPG_CharacterBase.generated.h"

class USphereComponent;
class USpringArmComponent;
class APlayerUnitBase;
class AEquipmentMeshBase;

UCLASS()
class JRPG_LEARN_API AJRPG_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AJRPG_CharacterBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetAI(bool state);
	virtual void SetPartyLeader(AJRPG_CharacterBase *Leader, int Index);
	virtual void RemoveAllEquipmentMesh();
	virtual void ShowEquipmentMesh(EEquipmentType, TSubclassOf<AEquipmentMeshBase>);

	FORCEINLINE bool GetIsControllingByAI() const { return bIsControllingByAI; }
	FORCEINLINE TSubclassOf<APlayerUnitBase> GetPlayerUnit() const {return PlayerUnit;}
	FORCEINLINE bool GetIsConntrollingByAI() const {return bIsControllingByAI;}
	FORCEINLINE USphereComponent* GetInteractionArea() const {return InteractionArea;}
	FORCEINLINE USphereComponent* GetBattleDetector() const {return BattleDetector;}
	
	protected:

	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent *SpringArmComponent;
	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent *CameraComponent;
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent *BattleDetector;
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent *InteractionArea;
	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent *PartyMember1SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent *PartyMember2SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
	UArrowComponent *PartyMember1Arrow;
	UPROPERTY(VisibleDefaultsOnly)
	UArrowComponent *PartyMember2Arrow;

	UPROPERTY(EditDefaultsOnly, Category = "Character | AI")
	float MinDistanceToPartyLeader = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Character | AI")
	float AcceptanceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY()
	class AAIController *AIController;
	UPROPERTY(EditDefaultsOnly)
	TMap<EEquipmentType, class AEquipmentMeshBase*> EquipmentMeshList;

	bool bIsControllingByAI;
	int PlayerIndex;

	TWeakObjectPtr<AJRPG_CharacterBase> PartyLeader;

	virtual FVector GetPartyMemberLocation(int Index) const;
	virtual void RemoveEquipmentMesh(EEquipmentType EquipmentType);
	
	void MoveToPartyLeader();

	void OnComponentBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void OnComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
};