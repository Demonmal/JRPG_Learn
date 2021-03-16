// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetIcon.h"
#include "Animation/WidgetAnimation.h"

void UTargetIcon::SetTargetIconVisibility(bool bIsVisible)
{
    if(bIsVisible)
    {
        PlayAnimation(MoveAnimation);
    }
    else
    {
        StopAnimation(MoveAnimation);
    }
}