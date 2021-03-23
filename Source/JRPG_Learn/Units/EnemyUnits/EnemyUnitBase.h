// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UnitBase.h"
#include "EnemyUnitBase.generated.h"

class ABattleSkillBase;
class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;

UCLASS()
class JRPG_LEARN_API AEnemyUnitBase : public AUnitBase
{
	GENERATED_BODY()
	public:
	AEnemyUnitBase();

	virtual void InitUnit(ABattleController* Controller) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void StartTurn() override;
	virtual void Die() override;

	void AdjustCameraForSkill(AUnitBase *TargetUnit);
	void AdjustFrontCamera();

	FORCEINLINE int GetExpReward() const {return ExpReward;}
	FORCEINLINE int GetGoldReward() const {return GoldReward;}
	FORCEINLINE TMap<TSubclassOf<AUsableItemBase>, float> GetUsableItemDrops() const {return UsableItemDrops;}
	FORCEINLINE TMap<TSubclassOf<AMiscItemBase>, float> GetMiscItemDrops() const {return MiscItemDrops;}
	FORCEINLINE TMap<TSubclassOf<AEquipmentBase>, float> GetEquipmentDrops() const {return EquipmentDrops;}

	protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UWidgetComponent* EnemyUnitWidget;

	void RotateUnitUIToCamera();
	void StopRotateUnitUIToCamera();
	TSubclassOf<ABattleSkillBase> GetRandomSkill();
	AUnitBase *GetTargetUnit(bool bIsPlayer);

	UPROPERTY(EditDefaultsOnly)
	int ExpReward;
	UPROPERTY(EditDefaultsOnly)
	int GoldReward;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<ABattleSkillBase>, float> Skills;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AUsableItemBase>, float> UsableItemDrops;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AMiscItemBase>, float> MiscItemDrops;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AEquipmentBase>, float> EquipmentDrops;

	UPROPERTY()
	class UEnemyUnitUI* EnemyUnitUI;

	bool bRotateUnitUIToCamera;
	FLatentActionInfo LatentActionInfo;
	
};
