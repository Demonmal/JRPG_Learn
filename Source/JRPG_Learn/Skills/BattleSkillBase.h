// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "../Data/Enums.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BattleSkillBase.generated.h"

class ABattleController;
class AUnitBase;
class UAnimMontage;
class APlayerUnitBase;
class ABuffBase;

UCLASS(Abstract)
class JRPG_LEARN_API ABattleSkillBase : public ASkillBase
{
	GENERATED_BODY()

public:
	FORCEINLINE ESkillType GetSkillType() const { return SkillType; }
	FORCEINLINE EDamageType GetDamageType() const { return DamageType; }

	void SetBattleController(ABattleController *Controller);

	virtual AUnitBase *GetTargetUnitForEnemy();

	virtual void UseBattleSkillInExplore(TSubclassOf<APlayerUnitBase> TargetUnit);
	virtual void PlaySkillAnimation();
	virtual void OnAnimMontageNotifyHandler(FName NotifyName, const FBranchingPointNotifyPayload &BranchingPointPayload);

	virtual void UseSkillOnNotify() {}
	virtual void OnSkillAnimationEnded() {}

protected:
	UPROPERTY(EditDefaultsOnly)
	ESkillType SkillType;
	UPROPERTY(EditDefaultsOnly)
	EDamageType DamageType;
	UPROPERTY(EditDefaultsOnly)
	bool bAlwaysHits;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AUnitBase>, UAnimMontage *> SkillAnimations;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ABuffBase>> Buffs;

	TWeakObjectPtr<ABattleController> BattleController;
	const FName UseSkillNotifyName = "UseSkill";
};
