// (c) XenFFly


#include "HelloNeighborRebornGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/JSON.h"
#include "Settings/SaveSystem.h"
	
void UHelloNeighborRebornGameInstance::Init()
{
	Super::Init();

	if (!IsValid(SaveSystem))
	{
		SaveSystem = NewObject<USaveSystem>(this);
		SaveSystem->Init(this);
	}

	if (!GameParams)
		LoadGameParams();
	if (!OtherSettings)
		LoadOtherSettings();

	if (!SaveSystem->IsLoadData())
		SaveSystem->LoadData();

	// Code...
}

void UHelloNeighborRebornGameInstance::Shutdown()
{
	Super::Shutdown();

	SaveGameParams();
	SaveOtherSettings();
}

void UHelloNeighborRebornGameInstance::SaveGameParams()
{
	if (SaveSystem)
	{
		FString Path = FString::Printf(TEXT("%s/SaveGames/gp.sav"), *FPaths::ProjectSavedDir()); 
		SaveSystem->SaveJsonGameFile(this, Path, GameParams->JsonParsed);
	}
}

void UHelloNeighborRebornGameInstance::LoadGameParams()
{
	if (SaveSystem)
	{
		GameParams = NewObject<UJSONLibrary>();
		FString Path = FString::Printf(TEXT("%s/SaveGames/gp.sav"), *FPaths::ProjectSavedDir()); 
		SaveSystem->LoadJsonGameFile(this, Path, GameParams->JsonParsed);
	}
}

void UHelloNeighborRebornGameInstance::SaveOtherSettings()
{
	if (SaveSystem)
	{
		OtherSettings->SetValueAsInteger("SlotSave", GetSaveSystem()->GetCurSlot());
		UJSONLibrary* Saves;
		OtherSettings->CreateObjectValue("Saves", Saves);
		
		TArray<FSlotData> AllSaves = GetSaveSystem()->GetAllSaves(false, false, false);
		for (int i = 0; i < AllSaves.Num(); i++)
		{
			Saves->SetValueAsString(FString::FromInt(i), AllSaves[i].FileName);
		}
		
		FString Path = FString::Printf(TEXT("%s/SaveGames/os.sav"), *FPaths::ProjectSavedDir()); 
		SaveSystem->SaveJsonGameFile(this, Path, OtherSettings->JsonParsed);
	}
}

void UHelloNeighborRebornGameInstance::LoadOtherSettings()
{
	if (SaveSystem)
	{
		OtherSettings = NewObject<UJSONLibrary>();
		FString Path = FString::Printf(TEXT("%s/SaveGames/os.sav"), *FPaths::ProjectSavedDir()); 
		SaveSystem->LoadJsonGameFile(this, Path, OtherSettings->JsonParsed);
	}
}

class USaveSystem* UHelloNeighborRebornGameInstance::GetSaveSystem() const
{
	return SaveSystem;
}

void UHelloNeighborRebornGameInstance::SaveSettingsGame()
{
	SaveGameParams();
	SaveOtherSettings();
}