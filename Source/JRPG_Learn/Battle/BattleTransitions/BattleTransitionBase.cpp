// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTransitionBase.h"

void ABattleTransitionBase::StartTransition()
{
	if (OnTransitionEnded.IsBound())
	{
		OnTransitionEnded.Broadcast();
	}
}

void ABattleTransitionBase::SetPlayerController(AJRPG_PlayerController *Controller)
{
	PlayerController = Controller;
}

void ABattleTransitionBase::SetBattle(ABattleBase *CurrentBattle)
{
	Battle = CurrentBattle;
}