// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillUsedSignature)

class AJRPG_PlayerController;

UCLASS()
class JRPG_LEARN_API ASkillBase : public AActor
{
	GENERATED_BODY()
public:
	FOnSkillUsedSignature OnSkillUsed;

	virtual void UseSkill();

	FORCEINLINE int GetManaRequired() const { return ManaRequired; }
	FORCEINLINE FText GetName() { return Name; }
	FORCEINLINE FText GetShortDescription() { return ShortDescription; }
	FORCEINLINE UTexture2D *GetIcon() { return Icon; }

	void SetPlayerController(AJRPG_PlayerController *Controller);

protected:

	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText ShortDescription;
	UPROPERTY(EditDefaultsOnly)
	FText FullDescription;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D *Icon;
	UPROPERTY(EditDefaultsOnly)
	int ManaRequired;
	UPROPERTY(EditDefaultsOnly)
	bool CanUseInExplore;

	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
};
