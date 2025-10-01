// (c) XenFFly

#include "RecoveryData.h"

#include "SaveSystem.h"
#include "../HelloNeighborRebornGameInstance.h"
#include "../HelloNeighborRebornGameMode.h"
#include "../Framework/JSON.h"
#include "../Gameplay/Objects/FlatActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "EngineUtils.h"

void URecoveryData::Init(class AHelloNeighborRebornGameMode* GameMode)
{
	GameMode->RecoveryData = this;
	pGameInstance = Cast<UHelloNeighborRebornGameInstance>(UGameplayStatics::GetGameInstance(this));
	pGameMode = GameMode;
}

void URecoveryData::PostLoad()
{
	UObject::PostLoad();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AFlatActor::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		AFlatActor* FlatActor = Cast<AFlatActor>(Actor);

		if (!IsValid(FlatActor))
			continue;

		FlatActor->OnPostLoad();
	}

	/*
	* Human and Sosed
	*
	AHuman* Human = AHuman::FindHuman(this, FGetHuman(FH_LOCAL)).Human;
	if (Human)
	{
		Human->LoadPost();
	}

	ASosed* Sosed = ASosed::FindSosed(this);
	if (Sosed)
	{
		Sosed->LoadPost();
	}
	*/
}

void URecoveryData::ClearSaveData()
{
	bIsLoadData = false;
	MarkPendingKill();
}

bool URecoveryData::IsLoadObjects() const
{
	return bIsLoadData;
}

void URecoveryData::SetLoadObjects(bool bLoad)
{
	bIsLoadData = bLoad;
}

void URecoveryData::LoadObjects()
{
	if (bIsLoadData)
		return;

	bIsLoadData = true;
	
	UJSONLibrary* LoadedSave = NewObject<UJSONLibrary>();
	
	if (!pGameInstance->GetSaveSystem()->m_oSlotData.FileName.IsEmpty())
	{
		USaveSystem* SaveSys = pGameInstance->GetSaveSystem();
		SaveSys->DecryptGameFile(SaveSys->m_oSlotData.FileName, LoadedSave->JsonParsed);
		SaveSys->m_oSlotData = FSlotData();
	}
	else pGameInstance->GetSaveSystem()->DownloadLastSave(LoadedSave->JsonParsed); 

	UE_LOG(LogShatalovSaves, Log, TEXT("URecoveryData::LoadObjects()"));

	// WIP...
}

void URecoveryData::Save(FString LevelName, FSaveParams Params, ESaveOwner Type)
{
	if (pGameInstance && pGameInstance->GetSaveSystem())
	{
		ensure(IsValid(pGameInstance));
		
		int32 Slot = 0;
		switch (Type) {
		case Auto:
			Slot = pGameInstance->GetSaveSystem()->GetCurSlot();
			break;
		case User:
			if (Params.SelectedSlot < 0)
			{
				UJSONLibrary* Saves;
				TArray<FString> Ids;
				
				pGameInstance->OtherSettings->GetValueAsObject("Saves", Saves);
				Saves->GetFieldNames(Ids);
				if (Ids.Num() > 0)
				{
					FString SlotName, Num;
					Saves->GetValueAsString(Ids.Last(), SlotName);
					SlotName.Split("_", nullptr, &Num);
					Slot = UKismetStringLibrary::Conv_StringToInt(Num) + 1;
				}
			}
			else Slot = Params.SelectedSlot;
			break;
		}

		UJSONLibrary* JsonSave = NewObject<UJSONLibrary>();

		// Current Level
		UJSONLibrary* CurrentLevel = NewObject<UJSONLibrary>();

		// Setting up CurrentLevel
		if (pGameInstance->GetSaveSystem()->DownloadLastSave(JsonSave->JsonParsed))
		{
			// Save project version if needed
			// JsonSave->SetValueAsString("Version", UHelloNeighborUtils::GetProjectVersion());

			UJSONLibrary* DateTimeObject;
			if (JsonSave->GetValueAsObject("DateTime", DateTimeObject))
			{
				DateTimeObject->SetValueAsString("Date", UKismetMathLibrary::Now().ToString(TEXT("%d.%m.%Y")));
				DateTimeObject->SetValueAsString("Time", UKismetMathLibrary::Now().ToString(TEXT("%H:%M:%S")));
			}

			JsonSave->SetValueAsString("StartMap", LevelName);
			
			UJSONLibrary* MapsObject;
			if (JsonSave->GetValueAsObject("Maps", MapsObject))
			{
				MapsObject->CreateObjectValue(LevelName, CurrentLevel);
				MapsObject = nullptr;
			}
		}
		else
		{
			// Save project version if needed
			// JsonSave->SetValueAsString("Version", UHelloNeighborUtils::GetProjectVersion());

			UJSONLibrary* DateTimeObject;
			if (JsonSave->CreateObjectValue("DateTime", DateTimeObject))
			{
				DateTimeObject->SetValueAsString("Date", UKismetMathLibrary::Now().ToString(TEXT("%d.%m.%Y")));
				DateTimeObject->SetValueAsString("Time", UKismetMathLibrary::Now().ToString(TEXT("%H:%M:%S")));
			}

			JsonSave->SetValueAsString("StartMap", LevelName);

			// Create Maps field-array
			UJSONLibrary* MapsObject;
			if (JsonSave->CreateObjectValue("Maps", MapsObject))
			{
				MapsObject->CreateObjectValue(LevelName, CurrentLevel);
				MapsObject = nullptr;
			}
		}

		// FlatActor's
		if (Params.bSaveObjects)
		{
			UJSONLibrary* ObjectsSave;
			CurrentLevel->CreateObjectValue("Objects", ObjectsSave);
			for (TActorIterator<AFlatActor> It(GetWorld(), AFlatActor::StaticClass()); It; ++It)
			{
				AFlatActor* FlatActor = *It;

				UJSONLibrary* FlatActorSave;
				ObjectsSave->CreateObjectValue(TSoftObjectPtr<AFlatActor>(FlatActor).ToString(), FlatActorSave);
					
				FlatActor->OnPreSave();
				FlatActor->Save(FlatActorSave);
			}
		}

		/* Save Neighbor and Human
		* 
		// Neighbor
		if (Params.bSaveNeighbor)
		{
			UJSONLibrary* NeighborSave;
			CurrentLevel->CreateObjectValue("Neighbor", NeighborSave);
			ASosed* Sosed = ASosed::FindSosed(this);
			if (IsValid(Sosed))
			{
				Sosed->OnSave(NeighborSave);
			}
		}

		// Human
		if (Params.bSavePlayer)
		{
			UJSONLibrary* HumanSave;
			CurrentLevel->CreateObjectValue("Human", HumanSave);
			AHuman* Human = AHuman::FindHuman(this, FGetHuman(FH_LOCAL)).Human;
			if (IsValid(Human))
			{
				Human->OnSave(HumanSave);
			}
		}
		*/

		pGameInstance->GetSaveSystem()->Save(Type, Slot, JsonSave->JsonParsed);
	}
}

void URecoveryData::DeleteSaveFile()
{
	pGameInstance->GetSaveSystem()->ClearCurrentSlot();
}

bool URecoveryData::LoadFile(FString Filename, UWorld* World)
{
	return false;
}
