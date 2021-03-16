// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSpawnLocation.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AUnitSpawnLocation::AUnitSpawnLocation()
{
	USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetCollisionProfileName(TEXT("NoCollision"));
	Sphere->bHiddenInGame = 1;
	RootComponent = Sphere;
	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->ArrowColor = FColor::Black;
	Arrow->SetupAttachment(RootComponent);
}