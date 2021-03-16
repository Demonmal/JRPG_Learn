// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoDialogue.h"

UInfoDialogue::UInfoDialogue() : UUIBase()
{
    bStopAction = false;
    bIsFocusable = true;
}

void UInfoDialogue::CallOnConfirmed()
{
    if(OnConfirmed.IsBound())
    {
        OnConfirmed.Broadcast();
    }
}