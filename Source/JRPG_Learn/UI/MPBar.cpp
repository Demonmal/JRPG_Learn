// Fill out your copyright notice in the Description page of Project Settings.


#include "MPBar.h"
#include "../Units/UnitBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMPBar::SetUnit(AUnitBase* Unit)
{
    CachedUnit = TWeakObjectPtr<AUnitBase>(Unit);
    CachedUnit->OnMPSet.AddUObject(this, &UMPBar::OnMPSetHandler);
    OnMPSetHandler(CachedUnit->GetCurrentMP());
}

void UMPBar::OnMPSetHandler(int MP)
{
    MPBar->SetPercent(MP/CachedUnit->GetMaxMP());
    MPText->SetText(FText::AsNumber(MP));
}
