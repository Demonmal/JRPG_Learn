// Fill out your copyright notice in the Description page of Project Settings.


#include "ThunderBolt.h"
#include "../Battle/BattleController.h"
#include "../Units/UnitBase.h"
#include "../Projectiles/ProjectileBase.h"

void AThunderBolt::UseSkill()
{
    bRangeAttackAnimHasEnded = false;
    bHasProjectileHit = false;
    PlaySkillAnimation();
}

void AThunderBolt::UseSkillOnNotify()
{   
    USceneComponent *ProjectileHitLocation = BattleController->GetCurrentAttackingUnit()->GetProjectileHitLocation();
    FTransform SpawnTransform = ProjectileHitLocation->GetComponentTransform();
    AProjectileBase *Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform);
    Projectile->SetTargetUnit(BattleController->GetCurrentTargetUnit());
    Projectile->OnProjectileHit.AddUObject(this, &AThunderBolt::OnProjectileHitHandler);
    Projectile->MoveProjectileToTarget();
}

void AThunderBolt::OnProjectileHitHandler()
{
    BattleController->DealDamage(Damage, 1.0f);
    bHasProjectileHit = true;
    if(bRangeAttackAnimHasEnded)
    {
        if(OnSkillUsed.IsBound())
        {
            OnSkillUsed.Broadcast();
        }
    }
}

void AThunderBolt::OnSkillAnimationEnded()
{
    bRangeAttackAnimHasEnded = true;
    if(bHasProjectileHit)
    {
        if(OnSkillUsed.IsBound())
        {
            OnSkillUsed.Broadcast();
        }
    }
}