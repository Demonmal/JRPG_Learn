// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillButton.h"
#include "../../Controllers/AudioPlayerController.h"
#include "../../Controllers/JRPG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "../../Skills/SkillBase.h"

void USkillButton::NativeConstruct()
{
	AudioPlayerController = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->AudioPlayerController(GetWorld());
	Super::NativeConstruct();
}

void USkillButton::SetSkillTextAndTexture()
{
    ASkillBase* SkillDefault = Cast<ASkillBase>(Skill->GetDefaultObject());
    SkillName->SetText(SkillDefault->GetName());
    SkillImage->SetBrushFromTexture(SkillDefault->GetIcon());
}

void USkillButton::SetMPAmount()
{
    ASkillBase* SkillDefault = Cast<ASkillBase>(Skill->GetDefaultObject());
    int ManaRequired = SkillDefault->GetManaRequired();
    FString String = FString::Printf(TEXT("%i MP"), ManaRequired);
    MPAmount->SetText(FText::FromString(String));
    ManaOverlay->SetVisibility(ManaRequired > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USkillButton::CallOnSkillButtonClicked()
{
    if(OnSkillButtonClicked.IsBound())
    {
        OnSkillButtonClicked.Broadcast(Skill, this);
    }
}

void USkillButton::PlayMenuNavigationSound()
{
     AudioPlayerController->PlaySoundByTag("MenuNavigation");
}