// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTimeBar.h"
#include "../Units/UnitBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UActionTimeBar::SetUnit(AUnitBase* Unit)
{
    CachedUnit = TWeakObjectPtr<AUnitBase>(Unit);
    CachedUnit->OnActionTimeAdded.AddUObject(this, &UActionTimeBar::OnActionTimeAddedHandler);
    CachedUnit->OnTurnEnded.AddUObject(this, &UActionTimeBar::OnTurnEndedHandler);
    CachedUnit->OnUnitDied.AddUObject(this, &UActionTimeBar::OnUnitDiedHandler);
    ResetActionTimeBar();
}

void UActionTimeBar::OnActionTimeAddedHandler(float ActionTime)
{
    float StartAtTime = UKismetMathLibrary::MapRangeClamped(CachedUnit->GetCurrentActionTime(), 0.f, CachedUnit->GetActionTime(), .0f, 1.0f);
    float EndAtTime = StartAtTime + UKismetMathLibrary::MapRangeClamped(ActionTime, 0.f, CachedUnit->GetActionTime(), .0f, 1.0f);
    float PlaybackSpeed = 1.0f / CachedUnit->GetActionTime();
    PlayAnimationTimeRange(Fill, StartAtTime, EndAtTime, 1, EUMGSequencePlayMode::Forward, PlaybackSpeed);
}

void UActionTimeBar::OnTurnEndedHandler()
{
    if(!CachedUnit->IsStuned())
    {
        ResetActionTimeBar();
    }
}

void UActionTimeBar::OnUnitDiedHandler(AUnitBase* DiedUnit)
{
    ResetActionTimeBar();
}

void UActionTimeBar::ResetActionTimeBar()
{
    ActionTimeBar->SetPercent(.0f);
    ReadyText->SetRenderOpacity(.0f);
}