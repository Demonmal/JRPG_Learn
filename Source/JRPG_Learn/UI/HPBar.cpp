// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"
#include "../Units/UnitBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHPBar::SetUnit(AUnitBase* Unit)
{
    CachedUnit = TWeakObjectPtr<AUnitBase>(Unit);
    CachedUnit->OnHPSet.AddUObject(this, &UHPBar::OnHPSetHandler);
    OnHPSetHandler(0);
}

void UHPBar::OnHPSetHandler(int HP)
{
    HPBar->SetPercent(HP/CachedUnit->GetMaxHP());
    HPText->SetText(FText::AsNumber(HP));
}