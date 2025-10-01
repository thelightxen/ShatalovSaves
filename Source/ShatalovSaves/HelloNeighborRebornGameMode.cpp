// (c) XenFFly

#include "HelloNeighborRebornGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Gameplay/Objects/FlatActor.h"
#include "Framework/JSON.h"
#include "HelloNeighborRebornGameInstance.h"
#include "Settings/SaveSystem.h"

void AHelloNeighborRebornGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_pGameInstance = Cast<UHelloNeighborRebornGameInstance>(UGameplayStatics::GetGameInstance(this));
	checkf(IsValid(m_pGameInstance), TEXT("m_pGameInstance"));

	// Create Autosave manager
	NewObject<URecoveryData>(this)->Init(this);

	// Code...
}

void AHelloNeighborRebornGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (RecoveryData && !RecoveryData->bIsLoadData)
	{
		RecoveryData->LoadObjects();
		RecoveryData->PostLoad();
	}

	// Code...
}

void AHelloNeighborRebornGameMode::SaveGame(int32 Slot)
{
	if (RecoveryData)
	{
		if (!bSaveCurrentLevel)
		{
		}
		RecoveryData->Save(GetWorld()->GetCurrentLevel()->GetOutermost()->GetPathName(), FSaveParams(Slot), User);
	}
}

void AHelloNeighborRebornGameMode::SaveLevel(FSaveParams SaveParams)
{
	if (RecoveryData)
	{
		if (!bSaveCurrentLevel)
		{
			SaveParams.bSaveObjects = false;
			SaveParams.bSavePlayer = false;
		}
		
		RecoveryData->Save(GetWorld()->GetCurrentLevel()->GetOutermost()->GetPathName(), SaveParams, Auto);
	}
}


bool AHelloNeighborRebornGameMode::LoadSaveFile(FString Filename)
{
	return false;
}

void AHelloNeighborRebornGameMode::LoadSaveData()
{
	if (!RecoveryData || RecoveryData->bIsLoadData)
		return;

	RecoveryData->LoadObjects();
	RecoveryData->PostLoad();
}

void AHelloNeighborRebornGameMode::SaveCurrentLevel(bool bSave)
{
	bSaveCurrentLevel = bSave;
}