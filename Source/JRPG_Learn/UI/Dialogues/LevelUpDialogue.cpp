// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpDialogue.h"

ULevelUpDialogue::ULevelUpDialogue() : UUIBase::UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void ULevelUpDialogue::OnConfirm()
{
    if(OnContinueClicked.IsBound())
    {
        OnContinueClicked.Broadcast();
    }
    PlayConfirmSound();
    RemoveFromParent();
}

void ULevelUpDialogue::ShowNewBattleSkills()
{

}

void ULevelUpDialogue::ShowNewExploreSkills()
{

}