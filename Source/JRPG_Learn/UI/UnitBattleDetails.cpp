// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitBattleDetails.h"
#include "../Units/UnitBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUnitBattleDetails::UpdateUnitDetails(AUnitBase* Unit)
{
    UE_LOG(LogTemp, Log, TEXT("UUnitBattleDetails::UpdateUnitDetails"))
    UnitName->SetText(Unit->GetTitle());
    UnitThumbnail->SetBrushFromTexture(Unit->GetThumbnail());
    FString String = FString::Printf(TEXT("Level %i"), Unit->GetUnitLevel());
    UnitLevel->SetText(FText::FromString(String));
    String = FString::Printf(TEXT("%i - %i"), Unit->GetMinAttack(), Unit->GetMaxAttack());
    PhysicalAttack->SetText(FText::FromString(String));
    String = FString::Printf(TEXT("%i"), Unit->GetDefense());
    Defense->SetText(FText::FromString(String));
    String = FString::Printf(TEXT("%i - %i"), Unit->GetMinMagicalAttack(), Unit->GetMaxMagicalAttack());
    MagicalAttack->SetText(FText::FromString(String));
    UE_LOG(LogTemp, Log, TEXT("MagicalAttack %i %s"), Unit->GetMinMagicalAttack() , *String)
    String = FString::Printf(TEXT("%i"), Unit->GetMagicDefense());
    MagicalDefense->SetText(FText::FromString(String));
    String = FString::Printf(TEXT("%i"), Unit->GetHit());
    Hit->SetText(FText::FromString(String));
    String = FString::Printf(TEXT("%i"), Unit->GetSpeed());
    Speed->SetText(FText::FromString(String));
}