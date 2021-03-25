// Fill out your copyright notice in the Description page of Project Settings.

#include "JRPG_PlayerController.h"
#include "JRPG_FunctionLibrary.h"
#include "JRPG_GameInstance.h"
#include "AudioPlayerController.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "../Characters/JRPG_CharacterBase.h"
#include "../Items/Equipment/EquipmentBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Quests/QuestBase.h"
#include "../Quests/ItemQuests/ItemQuestBase.h"
#include "../Interactables/InteractionDetector.h"
#include "../Items/Equipment/EquipmentBase.h"
#include "../Items/UsableItems/UsableItemBase.h"
#include "../Items/MiscItems/MiscItemBase.h"
#include "../Battle/BattleBase.h"
#include "TimerManager.h"

void AJRPG_PlayerController::SetPawn(APawn *InPawn)
{
    Super::SetPawn(InPawn);
    CachedCharacter = Cast<AJRPG_CharacterBase>(InPawn);
}

void AJRPG_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("PossessNextCharacter", EInputEvent::IE_Pressed, this, &AJRPG_PlayerController::PossessNextCharacter);
    InputComponent->BindAction("PossessPreviousCharacter", EInputEvent::IE_Pressed, this, &AJRPG_PlayerController::PossessPreviousCharacter);
    InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AJRPG_PlayerController::OnInteractedHandler);
    InputComponent->BindAxis("MoveForward", this, &AJRPG_PlayerController::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AJRPG_PlayerController::MoveRight);
    InputComponent->BindAxis("Turn", this, &AJRPG_PlayerController::Turn);
    InputComponent->BindAxis("LookUp", this, &AJRPG_PlayerController::LookUp);
    InputComponent->BindAxis("TurnRate", this, &AJRPG_PlayerController::TurnRate);
    InputComponent->BindAxis("LookUpRate", this, &AJRPG_PlayerController::LookUpRate);
}

void AJRPG_PlayerController::MoveForward(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid() && !FMath::IsNearlyZero(AxisValue, 1e-6f))
    {
        FRotator YawRotator = FRotator(0, GetControlRotation().Yaw, 0);
        FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
        CachedCharacter->AddMovementInput(ForwardVector, AxisValue);
    }
}

void AJRPG_PlayerController::MoveRight(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid() && !FMath::IsNearlyZero(AxisValue, 1e-6f))
    {
        FRotator YawRotator = FRotator(0, GetControlRotation().Yaw, 0);
        FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
        CachedCharacter->AddMovementInput(RightVector, AxisValue);
    }
}

void AJRPG_PlayerController::Turn(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->AddControllerYawInput(AxisValue);
    }
}

void AJRPG_PlayerController::LookUp(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->AddControllerPitchInput(AxisValue);
    }
}

void AJRPG_PlayerController::TurnRate(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    }
}

void AJRPG_PlayerController::LookUpRate(float AxisValue)
{
    if (bIsInputBlocked)
        return;
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->AddControllerPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
    }
}

void AJRPG_PlayerController::PossessNextCharacter()
{
    if (bIsInputBlocked)
        return;
    if (PartyMemberCharacters.Num() <= 0)
        return;
    int CurrentCharacterNumber;
    if (PartyMemberCharacters.Find(ExploreCharacter.Get(), CurrentCharacterNumber))
    {
        UnbindOverlapEvents();
        int NextCharacterNumber = CurrentCharacterNumber + 1 > PartyMemberCharacters.Num() - 1 ? 0 : CurrentCharacterNumber + 1;
        ExploreCharacter = PartyMemberCharacters[NextCharacterNumber];
        OnNewCharacterPossessed();
    }
}

void AJRPG_PlayerController::PossessPreviousCharacter()
{
    if (bIsInputBlocked)
        return;
    if (PartyMemberCharacters.Num() <= 0)
        return;
    int CurrentCharacterNumber;
    if (PartyMemberCharacters.Find(ExploreCharacter.Get(), CurrentCharacterNumber))
    {
        UnbindOverlapEvents();
        int PrevCharacterNumber = CurrentCharacterNumber - 1 >= 0 ? CurrentCharacterNumber - 1 : PartyMemberCharacters.Num() - 1;
        ExploreCharacter = PartyMemberCharacters[PrevCharacterNumber];
        OnNewCharacterPossessed();
    }
}

void AJRPG_PlayerController::OnInteractedHandler()
{
    if (bIsInputBlocked)
        return;
    if (Interactable.IsValid())
    {
        Interactable->OnInteract();
    }
}

void AJRPG_PlayerController::OnNewCharacterPossessed()
{
    if (OnPossessedCharacter.IsBound())
    {
        OnPossessedCharacter.Broadcast();
    }
    bIsInputBlocked = true;
    SetViewTargetWithBlend(ExploreCharacter.Get(), BlendTimeToNewCharacter, EViewTargetBlendFunction::VTBlend_Cubic, .0f, true);
    FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
        bIsInputBlocked = false;
        UnPossess();
        Possess(ExploreCharacter.Get());
        ExploreCharacter->SetAI(false);
        BindOverlapEvents();
        RefreshInteractions();
        RefreshBattleInteractions();
        SetPartyLeaderForAICharacters();
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, BlendTimeToNewCharacter, false);
}

void AJRPG_PlayerController::SetPartyLeaderForAICharacters()
{
    int Index = 0;
    for (int i{0}; i < PartyMemberCharacters.Num(); i++)
    {
        AJRPG_CharacterBase *PartyCharacter = PartyMemberCharacters[i];
        if (ExploreCharacter.Get() != PartyCharacter)
        {
            Index++;
            PartyCharacter->SetPartyLeader(ExploreCharacter.Get(), Index);
        }
    }
}

void AJRPG_PlayerController::BeginPlay()
{
    Super::BeginPlay();
    UJRPG_GameInstance *GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    AudioController = GameInstance->AudioPlayerController(GetWorld());
    SetUnitsStartingStats();
    SetPartyMembers();
    SetExploreCharacter();
}

bool AJRPG_PlayerController::TryGetUnitDataByPlayer(TSubclassOf<APlayerUnitBase> UnitClass, FPlayerUnitData &PlayerUnitData)
{
    bool Result = false;
    if (PlayerUnits.Contains(UnitClass))
    {
        Result = true;
        PlayerUnitData = PlayerUnits[UnitClass];
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AJRPG_PlayerController::TryGetUnitDataByPlayer PlayerUnits doesn't contain such unit: %s"), *UnitClass->GetName());
    }
    return Result;
}

void AJRPG_PlayerController::SetUnitsStartingStats()
{
    for (auto &Pair : PlayerUnits)
    {
        int Level;
        FUnitStats Stats = UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(Pair.Key, Pair.Value, Level);
        Pair.Value.CurrentHP = Stats.MaxHP;
        Pair.Value.CurrentMP = Stats.MaxMP;
    }
}

void AJRPG_PlayerController::SetPartyMembers()
{
    TArray<TSubclassOf<APlayerUnitBase>> Units;
    int KeysCount = PlayerUnits.GetKeys(Units);
    for (int i{0}; i < KeysCount; i++)
    {
        if (PartyMembers.Num() == PartySize)
            break;
        PartyMembers.Add(Units[i]);
    }
}

void AJRPG_PlayerController::SetExploreCharacter()
{
    switch (ExploreCharacterMode)
    {
    case EExploreCharacterMode::AllPartyMembers:
        ShowAllPartyMembers();
        break;
    case EExploreCharacterMode::FixedCharacter:
        ExploreCharacter = Cast<AJRPG_CharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        ExploreCharacter->SetAI(false);
        break;
    case EExploreCharacterMode::PartyLeader:
        ShowPartyLeader();
        break;
    }
}

void AJRPG_PlayerController::ShowPartyLeader()
{
    APlayerUnitBase *PartyLeaderDefault = Cast<APlayerUnitBase>(PartyMembers[0]->GetDefaultObject());
    if (ExploreCharacter.IsValid() && ExploreCharacter->GetClass() == PartyLeaderDefault->GetExploreCharacter())
    {
        ExploreCharacter->RemoveAllEquipmentMesh();
    }
    else
    {
        FTransform SpawnTransform = GetExploreCharacterTransform();
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AJRPG_CharacterBase *CreatedCharacter = GetWorld()->SpawnActor<AJRPG_CharacterBase>(PartyLeaderDefault->GetExploreCharacter(), SpawnTransform, SpawnParameters);
        if (ExploreCharacter.IsValid())
        {
            if (PartyMemberCharacters.Contains(ExploreCharacter.Get()))
            {
                PartyMemberCharacters.Remove(ExploreCharacter.Get());
            }
            ExploreCharacter->RemoveAllEquipmentMesh();
            ExploreCharacter->Destroy();
        }
        else
        {
            UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->Destroy();
        }
        ExploreCharacter = CreatedCharacter;
        Possess(CreatedCharacter);
    }
    ExploreCharacter->SetAI(false);
    if (!PartyMemberCharacters.Contains(ExploreCharacter.Get()))
    {
        PartyMemberCharacters.Add(ExploreCharacter.Get());
    }
    BindOverlapEvents();
    RefreshInteractions();
    ShowAllEquipmentMeshes(ExploreCharacter.Get());
}

void AJRPG_PlayerController::ShowAllPartyMembers()
{
    ShowPartyLeader();
    RemovePartyMemberCharacters();
    FTransform TransformToSpawn;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    float XOffset;
    float YOffset;
    FVector ExploreCharacherLocation = ExploreCharacter->GetActorLocation();
    FVector LocationToSpawn;
    LocationToSpawn.Z = ExploreCharacherLocation.Z;
    for (int i{1}; i < PartyMembers.Num(); i++)
    {
        APlayerUnitBase *UnitDefault = Cast<APlayerUnitBase>(PartyMembers[i]->GetDefaultObject());
        XOffset = UKismetMathLibrary::RandomFloatInRange(MinPartyMemberSpawnOffset, MaxPartyMemberSpawnOffset);
        YOffset = UKismetMathLibrary::RandomFloatInRange(MinPartyMemberSpawnOffset, MaxPartyMemberSpawnOffset);
        LocationToSpawn.X = ExploreCharacherLocation.X + XOffset;
        LocationToSpawn.Y = ExploreCharacherLocation.Y + YOffset;
        TransformToSpawn.SetLocation(LocationToSpawn);
        AJRPG_CharacterBase *NewPartyMember = GetWorld()->SpawnActor<AJRPG_CharacterBase>(UnitDefault->GetExploreCharacter(), TransformToSpawn, SpawnInfo);
        PartyMemberCharacters.Add(NewPartyMember);
        NewPartyMember->SetPartyLeader(ExploreCharacter.Get(), i);
        ShowAllEquipmentMeshes(NewPartyMember);
    }
}

void AJRPG_PlayerController::RemovePartyMemberCharacters()
{
    for (int i{PartyMemberCharacters.Num() - 1}; i >= 0; i--)
    {
        if (PartyMemberCharacters[i] != ExploreCharacter.Get())
        {
            if (IsValid(PartyMemberCharacters[i]))
            {
                PartyMemberCharacters[i]->RemoveAllEquipmentMesh();
                PartyMemberCharacters[i]->Destroy();
            }
            PartyMemberCharacters.RemoveAt(i);
        }
    }
}

EQuestStatus AJRPG_PlayerController::GetQuestStatus(TSubclassOf<AQuestBase> CurrentQuest)
{
    EQuestStatus QuestStatus = EQuestStatus::Unavailable;
    if (IsQuestAvailable(CurrentQuest))
    {
        AQuestBase *QuestDefault = Cast<AQuestBase>(CurrentQuest->GetDefaultObject());
        if (ActiveQuests.Contains(CurrentQuest))
        {
            QuestStatus = ActiveQuests[CurrentQuest] == QuestDefault->GetStepsCount() ? EQuestStatus::ReadyToDeliver : EQuestStatus::Active;
        }
        else
        {
            QuestStatus = CompletedQuests.Contains(CurrentQuest) && !QuestDefault->IsRepeatable() ? EQuestStatus::Completed : EQuestStatus::New;
        }
    }
    return QuestStatus;
}

bool AJRPG_PlayerController::IsQuestAvailable(TSubclassOf<AQuestBase> Quest)
{
    AQuestBase *QuestDefault = Cast<AQuestBase>(Quest->GetDefaultObject());
    bool IsAvailable = true;
    TArray<TSubclassOf<AQuestBase>> RequiredQuests = QuestDefault->RequiredQuests;
    for (int i{0}; i < RequiredQuests.Num(); i++)
    {
        if (!CompletedQuests.Contains(RequiredQuests[i]))
        {
            IsAvailable = false;
            break;
        }
    }
    return IsAvailable;
}

void AJRPG_PlayerController::ShowAllEquipmentMeshes(AJRPG_CharacterBase *PlayerCharacter)
{
    TSubclassOf<APlayerUnitBase> PlayerUnitClass = PlayerCharacter->GetPlayerUnit();
    if (UKismetSystemLibrary::IsValidClass(PlayerUnitClass))
    {
        FPlayerUnitData PlayerData = PlayerUnits[PlayerUnitClass];
        TArray<UClass *> EquipmentClasses = UJRPG_FunctionLibrary::GetAllEquipmentClasses(PlayerData);
        for (int i{0}; i < EquipmentClasses.Num(); i++)
        {
            AEquipmentBase *Equipment = Cast<AEquipmentBase>(EquipmentClasses[i]->GetDefaultObject());
            PlayerCharacter->ShowEquipmentMesh(Equipment->EquipmentType, EquipmentClasses[i]);
        }
    }
}

FTransform AJRPG_PlayerController::GetExploreCharacterTransform()
{
    FTransform Result;
    if (ExploreCharacter.IsValid())
    {
        Result = ExploreCharacter->GetActorTransform();
    }
    else
    {
        Result = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorTransform();
    }
    return Result;
}

void AJRPG_PlayerController::BindOverlapEvents()
{
    UnbindOverlapEvents();
    if (ExploreCharacter.IsValid())
    {
        ExploreCharacter->GetInteractionArea()->OnComponentBeginOverlap.AddDynamic(this, &AJRPG_PlayerController::OnComponentBeginOverlap);
        ExploreCharacter->GetInteractionArea()->OnComponentEndOverlap.AddDynamic(this, &AJRPG_PlayerController::OnComponentEndOverlap);
    }
}

void AJRPG_PlayerController::UnbindOverlapEvents()
{
    if (ExploreCharacter.IsValid())
    {
        ExploreCharacter->GetInteractionArea()->OnComponentBeginOverlap.RemoveAll(this);
        ExploreCharacter->GetInteractionArea()->OnComponentEndOverlap.RemoveAll(this);
    }
}

void AJRPG_PlayerController::OnComponentBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                                                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    DetectInteraction(OtherActor, OtherComp);
}

void AJRPG_PlayerController::OnComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    AInteractionDetector *InteractionDetector = Cast<AInteractionDetector>(OtherActor);
    if (InteractionDetector)
    {
        int First = InteractionDetector->GetInteractionSphere() == OverlappedComponent ? 1 : 0;
        int Second = Interactable.IsValid() ? 1 : 0;
        if (/*InteractionDetector->GetInteractionSphere() == OverlappedComponent &&*/ Interactable.IsValid())
        {
            RefreshInteractions();
        }
    }
}

void AJRPG_PlayerController::DetectInteraction(AActor *Actor, UPrimitiveComponent *Component)
{
    AInteractionDetector *InteractionDetector = Cast<AInteractionDetector>(Actor);
    if (InteractionDetector)
    {
        if (InteractionDetector->GetInteractionSphere() == Component && InteractionDetector->GetCanInteract())
        {
            InteractionDetector->SetPlayerController(this);
            if (InteractionDetector->IsForceInteraction())
            {
                Interactable = InteractionDetector;
                InteractionDetector->OnInteract();
            }
            else
            {
                if (!Interactable.IsValid())
                {
                    Interactable = InteractionDetector;
                    InteractionDetector->ShowInteractionUI();
                }
            }
        }
    }
}

void AJRPG_PlayerController::RefreshInteractions()
{
    if (Interactable.IsValid())
    {
        ClearInteractable();
    }
    TSet<UPrimitiveComponent *> OverlappingComponents;
    ExploreCharacter->GetInteractionArea()->GetOverlappingComponents(OverlappingComponents);
    for (auto Component : OverlappingComponents)
    {
        DetectInteraction(Component->GetOwner(), Component);
    }
}

void AJRPG_PlayerController::RefreshBattleInteractions()
{
    TSet<UPrimitiveComponent *> OverlappingComponents;
    ExploreCharacter->GetInteractionArea()->GetOverlappingComponents(OverlappingComponents);
    for (auto Component : OverlappingComponents)
    {
        ABattleBase *InteractedBattle = Cast<ABattleBase>(Component->GetOwner());
        if (IsValid(InteractedBattle))
        {
            InteractedBattle->OverlapStarted();
            break;
        }
    }
}

void AJRPG_PlayerController::ClearInteractable()
{
    Interactable->HideInteractionUI();
    Interactable = nullptr;
}

void AJRPG_PlayerController::AddAllUsableItemsToInventory(TMap<TSubclassOf<AUsableItemBase>, int> Items)
{
    for (const auto &Pair : Items)
    {
        AddUsableItemToInventory(Pair.Key, Pair.Value);
    }
}

void AJRPG_PlayerController::OnUsableItemUsed(TSubclassOf<AUsableItemBase> UsableItemClass, int AmountToUse)
{
    if (!UsableItems.Contains(UsableItemClass))
        return;
    int ItemCount = UsableItems[UsableItemClass];
    if (ItemCount - AmountToUse > 0)
    {
        UsableItems[UsableItemClass] = ItemCount - AmountToUse;
    }
    else
    {
        UsableItems.Remove(UsableItemClass);
    }
    UpdateItemQuests(UsableItemClass);
}

void AJRPG_PlayerController::AddAllMiscItemsToInventory(TMap<TSubclassOf<AMiscItemBase>, int> Items)
{
    for (const auto &Pair : Items)
    {
        AddMiscItemToInventory(Pair.Key, Pair.Value);
    }
}

void AJRPG_PlayerController::AddAllEquipmentToInventory(TMap<TSubclassOf<AEquipmentBase>, int> Items)
{
    for (const auto &Pair : Items)
    {
        AddEquipmentToInventory(Pair.Key, Pair.Value);
    }
}

void AJRPG_PlayerController::AddUsableItemToInventory(TSubclassOf<AUsableItemBase> Item, int Amount)
{
    if (UsableItems.Contains(Item))
    {
        UsableItems[Item] += Amount;
    }
    else
    {
        UsableItems.Add(Item, Amount);
    }
}

void AJRPG_PlayerController::AddMiscItemToInventory(TSubclassOf<AMiscItemBase> Item, int Amount)
{
    if (MiscItems.Contains(Item))
    {
        MiscItems[Item] += Amount;
    }
    else
    {
        MiscItems.Add(Item, Amount);
    }
}

void AJRPG_PlayerController::AddEquipmentToInventory(TSubclassOf<AEquipmentBase> Item, int Amount)
{
    if (Equipments.Contains(Item))
    {
        Equipments[Item] += Amount;
    }
    else
    {
        Equipments.Add(Item, Amount);
    }
}

void AJRPG_PlayerController::AddGold(float Amount)
{
    Gold += Amount;
}

void AJRPG_PlayerController::UpdateItemQuests(TSubclassOf<AItemBase> Item)
{
    TArray<TSubclassOf<AQuestBase>> Quests;
    int QuestsCount = ActiveQuests.GetKeys(Quests);
    for (int i{0}; i < QuestsCount; i++)
    {
        TSubclassOf<AQuestBase> Quest = Quests[i];
        if (Quest->IsChildOf(AItemQuestBase::StaticClass()))
        {
            AItemQuestBase *QuestDefault = Cast<AItemQuestBase>(Quest->GetDefaultObject());
            if (Item == QuestDefault->GetItemToCollect())
            {
                int Amount = GetItemAmount(Item);
                UpdateQuest(Quest, Amount, true, true);
            }
        }
    }
}

int AJRPG_PlayerController::GetItemAmount(TSubclassOf<AItemBase> Item)
{
    int Result = 0;
    if (Item->IsChildOf(AUsableItemBase::StaticClass()))
    {
        TSubclassOf<AUsableItemBase> UsableItem = *Item;
        if (UsableItems.Contains(UsableItem))
        {
            Result = UsableItems[UsableItem];
        }
    }
    else if (Item->IsChildOf(AMiscItemBase::StaticClass()))
    {
        TSubclassOf<AMiscItemBase> MiscItem = *Item;
        if (MiscItems.Contains(MiscItem))
        {
            Result = MiscItems[MiscItem];
        }
    }
    else if (Item->IsChildOf(AEquipmentBase::StaticClass()))
    {
        TSubclassOf<AEquipmentBase> Equipment = *Item;
        if (Equipments.Contains(Equipment))
        {
            Result = Equipments[Equipment];
        }
    }
    return Result;
}

void AJRPG_PlayerController::ChangeGameState(EGameState State)
{
    GameState = State;
    if (OnGameStateChanged.IsBound())
    {
        OnGameStateChanged.Broadcast(GameState);
    }
    switch (State)
    {
    case EGameState::Explore:
        DisableCharacter(false);
        SetBlockPlayerUnitState(false);
        break;
    case EGameState::Battle:
        DisableCharacter(true);
        SetBlockPlayerUnitState(true);
        break;
    }
}

void AJRPG_PlayerController::OnBattleOver()
{
    SetExploreCharacter();
    ChangeGameState(EGameState::Explore);
    RefreshBattleInteractions();
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);
}

void AJRPG_PlayerController::DisableCharacter(bool bIsDisabled)
{
    UJRPG_FunctionLibrary::SetActorDisableState(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), bIsDisabled);
}

void AJRPG_PlayerController::PlaySound2DByTag(FName Tag)
{
    AudioController->PlaySoundByTag(Tag);
}

void AJRPG_PlayerController::UpdatePlayerUnitsData(int TotalExp, TArray<APlayerUnitBase *> AliveUnits, TArray<TSubclassOf<APlayerUnitBase>> DeadUnits)
{
    FPlayerUnitData PlayerUnitData;
    for (auto Unit : AliveUnits)
    {
        if (TryGetUnitDataByPlayer(Unit->GetClass(), PlayerUnitData))
        {
            PlayerUnitData.Exp += TotalExp / AliveUnits.Num();
            PlayerUnitData.CurrentHP = Unit->GetCurrentHP();
            PlayerUnitData.CurrentMP = Unit->GetCurrentMP();
        }
    }
    for (const auto &DeadUnitClass : DeadUnits)
    {
        if (TryGetUnitDataByPlayer(DeadUnitClass, PlayerUnitData))
        {
            PlayerUnitData.CurrentHP = 0;
            PlayerUnitData.CurrentMP = 0;
        }
        RemoveFromParty(DeadUnitClass);
    }
}

void AJRPG_PlayerController::RemoveFromParty(TSubclassOf<APlayerUnitBase> Unit)
{
    PartyMembers.Remove(Unit);
}

void AJRPG_PlayerController::UpdateQuest(TSubclassOf<AQuestBase> Quest, int Steps, bool bOverrideProgress, bool bIgnoreNotification)
{
}