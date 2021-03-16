// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipmentMeshBase.generated.h"

class USkeletalMeshComponent;
class APlayerUnitBase;

UCLASS()
class JRPG_LEARN_API AEquipmentMeshBase : public AActor
{
	GENERATED_BODY()

public:

	void EquipmentOn(USkeletalMeshComponent* UnitMeshComponent);
	void EquipmentOff(USkeletalMeshComponent* UnitMeshComponent);

	void SetPlayerUnit(TSubclassOf<APlayerUnitBase>);

	FORCEINLINE FName GetExploreSocketName() const {return ExploreSocketName;}
	FORCEINLINE FName GetBattleSocketName() const {return BattleSocketName;}

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName BattleSocketName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ExploreSocketName;
};
