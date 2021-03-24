// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

class ABattleController;
class ABattleBase;
class UTurnOrderList;
class UUnitBattleDetails;
class USkillUseDialogue;
class UItemUseDialogue;
class UPlayerUnitsListUI;
class UBossUI;
class UWidgetAnimation;
class AItemBase;
class ABattleSkillBase;
class AUnitBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemSelectedSignature, TSubclassOf<AItemBase>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillSelectedSignature, TSubclassOf<ABattleSkillBase>);
DECLARE_MULTICAST_DELEGATE(FOnItemInventoryClosedSignature);
DECLARE_MULTICAST_DELEGATE(FOnItemUsedSignature);
DECLARE_MULTICAST_DELEGATE(FOnItemUseCancelledSignature);
DECLARE_MULTICAST_DELEGATE(FOnSkillUIClosedSignature);
DECLARE_MULTICAST_DELEGATE(FOnSkillUsedSignature);
DECLARE_MULTICAST_DELEGATE(FOnSkillUseCancelledSignature);

UCLASS()
class JRPG_LEARN_API UBattleUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ShowBattleUI();
	UFUNCTION(BlueprintCallable)
	void HideBattleUI();
	UFUNCTION(BlueprintCallable)
	void ShowBossUI();

	UFUNCTION(BlueprintCallable)
	void ShowUsableItemInventory();
	UFUNCTION(BlueprintCallable)
	void OnItemSelectedHandler(TSubclassOf<AItemBase> SelectedItemClass);
	UFUNCTION(BlueprintCallable)
	void OnItemUsedHandler(TSubclassOf<AItemBase> SelectedItemClass);
	UFUNCTION(BlueprintCallable)
	void OnItemUseCancelledHandler();
	UFUNCTION(BlueprintCallable)
	void OnItemCancelledHandler();
	UFUNCTION(BlueprintCallable)
	void HideUsableItemInventory();
	UFUNCTION(BlueprintCallable)
	void HideItemsActionButtons();
	UFUNCTION(BlueprintCallable)
	void UnbindAllItemEvents();
	UFUNCTION(BlueprintCallable)
	void UseItem();

	UFUNCTION(BlueprintCallable)
	void ShowSkills(APlayerUnitBase *PlayerUnit);
	UFUNCTION(BlueprintCallable)
	void OnSkillSelectedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill);
	UFUNCTION(BlueprintCallable)
	void OnSkillCancelledHandler();
	UFUNCTION(BlueprintCallable)
	void OnUnitHasEnoughMP();
	UFUNCTION(BlueprintCallable)
	void OnSkillUsedHandler(TSubclassOf<ABattleSkillBase> SelectedSkill);
	UFUNCTION(BlueprintCallable)
	void OnSkillUseCancelledHandler();
	UFUNCTION(BlueprintCallable)
	void UnbindAllSkillEvents();
	UFUNCTION(BlueprintCallable)
	void HideSkills();
	UFUNCTION(BlueprintCallable)
	void HideSkillActionButtons();

	UFUNCTION(BlueprintCallable)
	void ShowUnitBattleDetails();
	UFUNCTION(BlueprintCallable)
	void HideUnitBattleDetails();
	UFUNCTION(BlueprintCallable)
	void UpdateAttackingUnitBattleDetails(AUnitBase *Unit);
	UFUNCTION(BlueprintCallable)
	void UpdateTargetUnitBattleDetails(AUnitBase *Unit);
	UFUNCTION(BlueprintCallable)
	void UpdateTurnOrderList(AUnitBase *Unit);
	UFUNCTION(BlueprintCallable)
	void InitTurnOrderUI();

	void SetBattle(ABattleBase *Battle);
	void SetBattleController(ABattleController *Controller);

	FOnItemSelectedSignature OnItemSelected;
	FOnSkillSelectedSignature OnSkillSelected;
	FOnItemInventoryClosedSignature OnItemInventoryClosed;
	FOnItemUsedSignature OnItemUsed;
	FOnItemUseCancelledSignature OnItemUseCancelled;
	FOnSkillUIClosedSignature OnSkillUIClosed;
	FOnSkillUsedSignature OnSkillUsed;
	FOnSkillUseCancelledSignature OnSkillUseCancelled;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUnitBattleDetails *AttackingUnitBattleDetails;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUnitBattleDetails *TargetUnitBattleDetails;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBossUI *BP_BossUI;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UItemUseDialogue *BP_ItemUseDialogue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillUseDialogue *BP_SkillUseDialogue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlayerUnitsListUI *BP_PlayerUnitsList;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTurnOrderList *BP_TurnOrderList;

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *ShowTurnOrderUI;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *ShowBossUIAnim;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *TargetImageAnim;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *ShowUnitBattleDetailsAnim;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *ShowPlayerUnitListUIAnim;

private:

	TWeakObjectPtr<ABattleBase> CurrentBattle;
	TWeakObjectPtr<ABattleController> BattleController;

	bool bIsUnitBattleDetailsShown;
	bool bIsTurnOrderInited;
};