// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUseDialogue.h"
#include "SkillButton.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "../ActionButton.h"
#include "../../Skills/SkillBase.h"
#include "../../Skills/BattleSkillBase.h"
#include "Kismet/KismetArrayLibrary.h"

USkillUseDialogue::USkillUseDialogue() : UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void USkillUseDialogue::ShowSkills()
{
    PlayShowSkillsAnim(EUMGSequencePlayMode::Type::Forward);
    PlaySkillActionButtonsAnim(EUMGSequencePlayMode::Type::Forward);
    FocusOnThisUI();
}

void USkillUseDialogue::PlayShowSkillsAnim(EUMGSequencePlayMode::Type PlayMode)
{
    PlayAnimation(ShowSkillAnim, .0f, 1, PlayMode);
}

void USkillUseDialogue::PlaySkillActionButtonsAnim(EUMGSequencePlayMode::Type PlayMode)
{
    PlayAnimation(ShowSkillActionButtonsAnim, .0f, 1, PlayMode);
}

void USkillUseDialogue::HighlightSelectedSkill()
{
    HighlightedSkill = SkillButtons[SelectedSkillIndex];
    HighlightedSkill->Highlight(true);
    SkillDescriptionOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
    ASkillBase* SkillDefault = Cast<ASkillBase>(HighlightedSkill->Skill->GetDefaultObject());
    SkillDescriptionText->SetText(SkillDefault->GetShortDescription());
    EmptyText->SetVisibility(ESlateVisibility::Hidden);
}

void USkillUseDialogue::CheckIfSelectedSkillAndHighlightNewOne()
{
    if(IsValid(HighlightedSkill))
    {
        HighlightedSkill->Highlight(false);
    }
    HighlightSelectedSkill();
}

void USkillUseDialogue::SetSkills(TArray<TSubclassOf<ABattleSkillBase>> Skills)
{
    EmptyText->SetVisibility(ESlateVisibility::HitTestInvisible);
    HighlightedSkill = nullptr;
    SkillDescriptionOverlay->SetVisibility(ESlateVisibility::Collapsed);
    SkillList->ClearChildren();
    SkillButtons.Empty();
    for(const auto& Skill : Skills)
    {
        USkillButton* Button = CreateWidget<USkillButton>(GetWorld(), SkillButtonClass);
        SkillList->AddChild(Button);
        SkillButtons.Add(Button);
        Button->OnSkillButtonClicked.AddUObject(this, &USkillUseDialogue::OnSkillButtonClickedHandler);
    }
    SelectFirstItem();
    ConfirmButton->OnActionButtonClicked.AddUObject(this, &USkillUseDialogue::Confirm);
    CancelButton->OnActionButtonClicked.AddUObject(this, &USkillUseDialogue::Cancel);
}

void USkillUseDialogue::OnSkillButtonClickedHandler(TSubclassOf<ASkillBase> SkillClass, USkillButton* SkillButton)
{
    SelectedSkillIndex = SkillButtons.Find(SkillButton);
    CheckIfSelectedSkillAndHighlightNewOne();
}

void USkillUseDialogue::CallOnSkillSelected(TSubclassOf<ABattleSkillBase> SelectedSkill)
{
    if(OnSkillSelected.IsBound())
    {
        OnSkillSelected.Broadcast(SelectedSkill);
    }
}

void USkillUseDialogue::CallOnCanceled()
{
    if(OnCanceled.IsBound())
    {
        OnCanceled.Broadcast();
    }
}