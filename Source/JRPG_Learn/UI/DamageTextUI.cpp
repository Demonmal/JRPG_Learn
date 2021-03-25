// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextUI.h"
#include "Animation/WidgetAnimation.h"

void UDamageTextUI::SetDamage(int Amount_l, AUnitBase *TargetUnit_l, bool bIsHeal_l, bool bIsMPRestore_l)
{
    Amount = Amount_l;
    TargetUnit = TargetUnit_l;
    bIsHeal = bIsHeal_l;
    bIsMPRestore = bIsMPRestore_l;
    SetDamageText();
    SetDamageColor();
    PlayAnimation(Bounce);
}

void UDamageTextUI::UnbindAllFromAppearAnimation()
{
    //Appear->UnbindAllFromAnimationFinished(this);
}
