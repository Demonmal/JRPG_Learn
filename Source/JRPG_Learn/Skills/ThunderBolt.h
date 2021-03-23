// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleSkillBase.h"
#include "ThunderBolt.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API AThunderBolt : public ABattleSkillBase
{
	GENERATED_BODY()
	public:
	
	virtual void UseSkill() override;
	virtual void UseSkillOnNotify() override;
	virtual void OnSkillAnimationEnded() override;

	private:

	void OnProjectileHitHandler();

	UPROPERTY(EditDefaultsOnly)
	int Damage = 60;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	bool bRangeAttackAnimHasEnded;
	bool bHasProjectileHit;
};
