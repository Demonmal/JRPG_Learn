// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "../Controllers/JRPG_PlayerController.h"

void ASkillBase::UseSkill()
{
	if(OnSkillUsed.IsBound())
	{
		OnSkillUsed.Broadcast();
	}
}

void ASkillBase::SetPlayerController(AJRPG_PlayerController *Controller)
{
	PlayerController = TWeakObjectPtr<AJRPG_PlayerController>(Controller);
}
