#include "BattleController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Controllers/JRPG_GameInstance.h"

ABattleController::ABattleController()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	BattleThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleThemeAudio"));
	BattleThemeAudio->SetupAttachment(RootComponent);
	BattleThemeAudio->bAutoActivate = 0;

	VictoryThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("VictoryThemeAudio"));
	VictoryThemeAudio->SetupAttachment(RootComponent);
	VictoryThemeAudio->bAutoActivate = 0;

	ExploreThemeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ExploreThemeAudio"));
	ExploreThemeAudio->SetupAttachment(RootComponent);
	ExploreThemeAudio->bAutoActivate = 0;
}

void ABattleController::BeginPlay()
{
	Super::BeginPlay();
	PlayExploreTheme();
	UJRPG_GameInstance *GameInstance = Cast<UJRPG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		if (GameInstance->GetShouldLoadGame())
		{
			FLatentActionInfo LatentActionInfo;
			UKismetSystemLibrary::Delay(GetWorld(), 0.1f, LatentActionInfo);
			GameInstance->LoadGame();
		}
	}
}

void ABattleController::PlayExploreTheme()
{
	if (!IsValid(ExploreTheme))
		return;
	ExploreThemeAudio->SetSound(ExploreTheme);
	ExploreThemeAudio->FadeIn(1.0f, 1.0f);
}