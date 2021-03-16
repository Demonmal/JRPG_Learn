// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentMeshBase.h"

// Called when the game starts or when spawned
void AEquipmentMeshBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipmentMeshBase::SetPlayerUnit(TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
	PlayerUnit = PlayerUnitClass;
}

void AEquipmentMeshBase::EquipmentOn(USkeletalMeshComponent* UnitMeshComponent)
{

}

void AEquipmentMeshBase::EquipmentOff(USkeletalMeshComponent* UnitMeshComponent)
{

}