// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

void ASkillBase::UseSkill()
{
	if(OnSkillUsed.IsBound())
	{
		OnSkillUsed.Broadcast();
	}
}
