// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleController.generated.h"

class UAudioComponent;
class AJRPG_PlayerController;

UCLASS()
class JRPG_LEARN_API ABattleController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleController();

	FORCEINLINE AJRPG_PlayerController* GetPlayerController() const { return PlayerController; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *BattleThemeAudio;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *VictoryThemeAudio;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent *ExploreThemeAudio;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase *ExploreTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase *BattleTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase *VictoryTheme;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase *DefeatTheme;

	UPROPERTY(VisibleAnywhere)
	AJRPG_PlayerController *PlayerController;

	void PlayExploreTheme();
};
