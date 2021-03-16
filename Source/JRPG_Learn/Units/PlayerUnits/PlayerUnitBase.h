// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UnitBase.h"
#include "../../Data/Enums.h"
#include "PlayerUnitBase.generated.h"

class ABattleSkillBase;
class AExploreSkillBase;
class AJRPG_CharacterBase;
class UTexture2D;

UCLASS()
class JRPG_LEARN_API APlayerUnitBase : public AUnitBase
{
	GENERATED_BODY()
	
	public:
	APlayerUnitBase();

	virtual void InitUnit(ABattleController* Controller) override;
	virtual void SetUnitStats() override;

	FORCEINLINE float GetExpExponent() const { return ExpExponent; }
	FORCEINLINE EUnitClass GetUnitClass() const { return UnitClass; }
	FORCEINLINE TSubclassOf<AJRPG_CharacterBase> GetExploreCharacter() const { return ExploreCharacter; }
	FORCEINLINE TMap<TSubclassOf<AExploreSkillBase>, int> GetExploreSkills() const { return ExploreSkills; }
	FORCEINLINE TMap<TSubclassOf<ABattleSkillBase>, int> GetBattleSkills() const { return BattleSkills; }
	FORCEINLINE UTexture2D * GetBattleImage() const { return BattleImage; }

	void SetPlayerHPMP();
	void SetBattleSkills();
	void AdjustBackCamera(AUnitBase* TargetUnit, bool bIsInstant);

	protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UWidgetComponent* ActionButtonsWidget;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UArrowComponent* BackCameraArrow;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BigImage;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BattleImage;

	UPROPERTY(EditDefaultsOnly)
	float ExpExponent = 15.f;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<ABattleSkillBase>, int> BattleSkills;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AExploreSkillBase>, int> ExploreSkills;	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AJRPG_CharacterBase> ExploreCharacter;
	UPROPERTY(EditDefaultsOnly)
	EUnitClass UnitClass;
	UPROPERTY()
	TArray<TSubclassOf<ABattleSkillBase>> AvailableBattleSkills;

	FTransform CameraStartTransform;

	TWeakObjectPtr<class UActionsUI> ActionsUI;
};