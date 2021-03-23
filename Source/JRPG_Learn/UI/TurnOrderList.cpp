// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnOrderList.h"
#include "../Battle/BattleController.h"
#include "../Units/UnitBase.h"

void UTurnOrderList::InitTurnOrderList(ABattleController *Controller)
{
    BattleController = TWeakObjectPtr<ABattleController>(Controller);
}

void UTurnOrderList::SetRevivedUnit(AUnitBase *Unit)
{
    RevivedUnit = TWeakObjectPtr<AUnitBase>(Unit);
}

void UTurnOrderList::UpdateTurnOderList(AUnitBase* CurrentUnit)
{

}
