// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageText.h"
#include "Components/WidgetComponent.h"
#include "../UI/DamageTextUI.h"
#include "TimerManager.h"

ADamageText::ADamageText()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DamageTextUIWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageTextUIWidget"));
    DamageTextUIWidget->SetWidgetSpace(EWidgetSpace::Screen);
    DamageTextUIWidget->SetupAttachment(RootComponent);
}

void ADamageText::CreateTextWidget(int Amount, class AUnitBase *TargetUnit, bool bIsHeal, bool bIsMPRestore)
{
	if(IsValid(DamageTextUI))
		return;
	DamageTextUIWidget->InitWidget();
	DamageTextUI = Cast<UDamageTextUI>(DamageTextUIWidget->GetUserWidgetObject());
	FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
        DamageTextUI->UnbindAllFromAppearAnimation();
		Destroy();
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, BounceAnimationWaitTime, false);
	DamageTextUI->SetDamage(Amount, TargetUnit, bIsHeal, bIsMPRestore);
}
