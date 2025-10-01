// (c) XenFFly


#include "SaveSystem.h"

#include "../HelloNeighborRebornGameInstance.h"
#include "../Framework/JSON.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"

void USaveSystem::Init(class UHelloNeighborRebornGameInstance* GameInstance)
{
	pGameInstance = GameInstance;

	checkf(IsValid(pGameInstance), TEXT("pGameInstance == nullptr"));
	
	GetWorld()->GetTimerManager().SetTimer(ChangeSlotTimer, this, &USaveSystem::ChangeSaveSlot,
	                                       pGameInstance->TimeAutosave, true);
}

bool USaveSystem::SaveJsonGameFile(class UHelloNeighborRebornGameInstance* GameInstance, const FString& Path,
                                   TSharedPtr<FJsonObject> JsonObject)
{
	if (Path.IsEmpty())
		return false;
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
		return false;

	FString Base64String = FBase64::Encode(JsonString);
	if (!FFileHelper::SaveStringToFile(Base64String, *Path))
	{
		return false;	
	}

	return true;
}

bool USaveSystem::LoadJsonGameFile(class UHelloNeighborRebornGameInstance* GameInstance, const FString& Path,
	TSharedPtr<FJsonObject>& JsonObject)
{
	if (Path.IsEmpty())
		return false;
	
	FString Base64String;
	if (!FFileHelper::LoadFileToString(Base64String, *Path))
	{
		return false;
	}
			
	FString DecodedJson;
	if (!FBase64::Decode(Base64String, DecodedJson))
		return false;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DecodedJson);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		return false;

	return true;
}

FSlotData USaveSystem::GetInfoSaveFile(FString SaveName)
{
	if (AutosavesData.Contains(SaveName))
		return AutosavesData[SaveName];
	else if (UsersavesData.Contains(SaveName))
		return UsersavesData[SaveName];
	else
	{
		UE_LOG(LogShatalovSaves, Error, TEXT("Save file not found"));
		return FSlotData();
	};
}

TArray<FSlotData> USaveSystem::GetAllSaves(bool bIncludeAutoSave, bool bSort, bool bCheckExists)
{
	TArray<FSlotData> Result;
	UsersavesData.GenerateValueArray(Result);

	if (bIncludeAutoSave)
	{
		TArray<FSlotData> Autosaves;
		AutosavesData.GenerateValueArray(Autosaves);
		Result.Append(Autosaves);
	}

	if (Result.Num() == 0)
		return Result;

	if (bCheckExists)
	{
		Result.RemoveAll([](const FSlotData& Element)
		{
			return !FPlatformFileManager::Get().GetPlatformFile().FileExists(
			*FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir() , *Element.FileName));
		});
	}
	
	if (bSort)
	{
		Algo::Sort(Result, [](const FSlotData& A, const FSlotData& B)
		{
			return A.GetDateTime() > B.GetDateTime();
		});
	}
	
	return Result;
}

int32 USaveSystem::GetSaveCount() const
{
	return AutosavesData.Num() + UsersavesData.Num();
}

bool USaveSystem::DecryptGameFile(const FString& SaveName, TSharedPtr<FJsonObject>& JsonObject)
{
	if (SaveName.IsEmpty())
		return false;

	const FString FullPath = FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), *SaveName);

	TArray<uint8> EncryptedData;
	if (!FFileHelper::LoadFileToArray(EncryptedData, *FullPath))
		return false;

	if (EncryptedData.Num() == 0)
		return false;

	FAES::DecryptData(EncryptedData.GetData(), EncryptedData.Num(), TCHAR_TO_ANSI(*pGameInstance->EncryptionJsonKey));

	if (EncryptedData.Num() > 0)
	{
		const int32 PaddingSize = EncryptedData.Last();
		if (PaddingSize > 0 && PaddingSize <= FAES::AESBlockSize)
		{
			bool bIsValidPadding = true;
			for (int32 i = 0; i < PaddingSize; ++i)
			{
				if (EncryptedData[EncryptedData.Num() - 1 - i] != PaddingSize)
				{
					bIsValidPadding = false;
					break;
				}
			}

			if (bIsValidPadding)
			{
				EncryptedData.SetNum(EncryptedData.Num() - PaddingSize, false);
			}
		}
	}
	
	FUTF8ToTCHAR Converter(reinterpret_cast<const char*>(EncryptedData.GetData()), EncryptedData.Num());
	FString Base64String(Converter.Length(), Converter.Get());
	
	FString JsonString;
	if (!FBase64::Decode(Base64String, JsonString))
		return false;
	
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		return false;

	return true;
}

bool USaveSystem::Save(ESaveOwner Type, int Slot, const TSharedPtr<FJsonObject>& JsonObject)
{
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		OnSaveGame.Broadcast(false);
		return false;
	}

	FString Base64String = FBase64::Encode(JsonString);

	TArray<uint8> DataToEncrypt;
	FTCHARToUTF8 Converter(*Base64String);
	DataToEncrypt.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
	
	const int32 DataSize = DataToEncrypt.Num();
	const int32 PaddingSize = FAES::AESBlockSize - (DataSize % FAES::AESBlockSize);
	for (int32 i = 0; i < PaddingSize; ++i)
	{
		DataToEncrypt.Add(PaddingSize);
	}
	
	FAES::EncryptData(DataToEncrypt.GetData(), DataToEncrypt.Num(), TCHAR_TO_ANSI(*pGameInstance->EncryptionJsonKey));

	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames");
	FString FileName;

	UJSONLibrary* SaveData = NewObject<UJSONLibrary>();
	UJSONLibrary* SaveDateTime;
	FSlotData NewData;
	
	switch (Type)
	{
	case Auto:
		FileName = FString::Printf(TEXT("autosave%d"), Slot);
		
		// Update AutosavesData
		SaveData->JsonParsed = JsonObject;
		
		SaveData->GetValueAsInteger("Version", NewData.Version);
		SaveData->GetValueAsString("StartMap", NewData.LevelName);
		NewData.FileName = FileName;
		NewData.bAutosave = true;
		
		if (SaveData->GetValueAsObject("DateTime", SaveDateTime))
		{
			SaveDateTime->GetValueAsString("Date", NewData.Date);
			SaveDateTime->GetValueAsString("TIme", NewData.Time);
		}
		AutosavesData.Add(FileName, NewData);
		break;
	case User:
		FileName = FString::Printf(TEXT("sf_%d"), Slot);

		// Update UsersavesData
		SaveData->JsonParsed = JsonObject;
		
		SaveData->GetValueAsInteger("Version", NewData.Version);
		SaveData->GetValueAsString("StartMap", NewData.LevelName);
		NewData.FileName = FileName;
		NewData.bAutosave = false;
		
		if (SaveData->GetValueAsObject("DateTime", SaveDateTime))
		{
			SaveDateTime->GetValueAsString("Date", NewData.Date);
			SaveDateTime->GetValueAsString("TIme", NewData.Time);
		}
		UsersavesData.Add(FileName, NewData);
		break;
	}
	
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDirectory);
	if (!FFileHelper::SaveArrayToFile(DataToEncrypt, *FString::Printf(TEXT("%s/%s.sav"), *SaveDirectory, *FileName)))
	{
		OnSaveGame.Broadcast(false);
		return false;
	}
	pGameInstance->SaveGameParams();
	pGameInstance->SaveOtherSettings();
	
	OnSaveGame.Broadcast(true);
	return true;
}

void USaveSystem::DeleteSave(FString SaveName)
{
	if (GetInfoSaveFile(SaveName).bAutosave)
		AutosavesData.Remove(SaveName);
	else UsersavesData.Remove(SaveName);
	
	FString FullPath = FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), *SaveName);
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FullPath);

	pGameInstance->SaveGameParams();
	pGameInstance->SaveOtherSettings();
}

bool USaveSystem::DecryptGameFileAsString(const FString& SaveName, FString& JsonString)
{
	if (SaveName.IsEmpty())
		return false;

	const FString FullPath = FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), *SaveName);

	TArray<uint8> EncryptedData;
	if (!FFileHelper::LoadFileToArray(EncryptedData, *FullPath))
		return false;

	if (EncryptedData.Num() == 0)
		return false;

	FAES::DecryptData(EncryptedData.GetData(), EncryptedData.Num(), TCHAR_TO_ANSI(*pGameInstance->EncryptionJsonKey));

	if (EncryptedData.Num() > 0)
	{
		const int32 PaddingSize = EncryptedData.Last();
		if (PaddingSize > 0 && PaddingSize <= FAES::AESBlockSize)
		{
			bool bIsValidPadding = true;
			for (int32 i = 0; i < PaddingSize; ++i)
			{
				if (EncryptedData[EncryptedData.Num() - 1 - i] != PaddingSize)
				{
					bIsValidPadding = false;
					break;
				}
			}

			if (bIsValidPadding)
			{
				EncryptedData.SetNum(EncryptedData.Num() - PaddingSize, false);
			}
		}
	}
	
	FUTF8ToTCHAR Converter(reinterpret_cast<const char*>(EncryptedData.GetData()), EncryptedData.Num());
	FString Base64String(Converter.Length(), Converter.Get());
	
	if (!FBase64::Decode(Base64String, JsonString))
		return false;

	return true;
}

bool USaveSystem::DownloadLastSave(TSharedPtr<FJsonObject>& JsonObject)
{
	TArray<FSlotData> Saves = GetAllSaves(true, true);

	if (Saves.Num() == 0)
		return false;

	FString FileName;
	if (!m_oSlotData.FileName.IsEmpty())
		FileName = m_oSlotData.FileName;
	else FileName = Saves[0].FileName;

	return DecryptGameFile(FileName, JsonObject);
}

void USaveSystem::LoadData()
{
	// Load Autosaves
	for (int i = 0; i <= 3; i++)
	{
		FString FileName = FString::Printf(TEXT("%s%d"), *AutosaveName, i);
		
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(
			*FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir() , *FileName)))
			continue;

		UJSONLibrary* Autosave = NewObject<UJSONLibrary>();
		DecryptGameFile(FileName, Autosave->JsonParsed);

		FSlotData NewData;
		
		Autosave->GetValueAsInteger("Version", NewData.Version);
		Autosave->GetValueAsString("StartMap", NewData.LevelName);
		NewData.FileName = FileName;
		NewData.bAutosave = true;

		UJSONLibrary* DateTimeObject;
		if (Autosave->GetValueAsObject("DateTime", DateTimeObject))
		{
			DateTimeObject->GetValueAsString("Date", NewData.Date);
			DateTimeObject->GetValueAsString("TIme", NewData.Time);
		}

		AutosavesData.Add(FileName, NewData);
	}

	// Load Usersaves
	if (pGameInstance->OtherSettings->HasField("Saves"))
	{
		UJSONLibrary* Saves;
		pGameInstance->OtherSettings->GetValueAsObject("Saves", Saves);

		TArray<FString> SavesNames;
		Saves->GetFieldNames(SavesNames);
		for (FString SaveIndex : SavesNames)
		{
			FString FileName;
			Saves->GetValueAsString(SaveIndex, FileName);

			if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(
			*FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir() , *FileName)))
				continue;

			UJSONLibrary* Usersave = NewObject<UJSONLibrary>();
			DecryptGameFile(FileName, Usersave->JsonParsed);

			FSlotData NewData;
		
			Usersave->GetValueAsInteger("Version", NewData.Version);
			Usersave->GetValueAsString("StartMap", NewData.LevelName);
			NewData.FileName = FileName;
			NewData.bAutosave = false;

			UJSONLibrary* DateTimeObject;
			if (Usersave->GetValueAsObject("DateTime", DateTimeObject))
			{
				DateTimeObject->GetValueAsString("Date", NewData.Date);
				DateTimeObject->GetValueAsString("TIme", NewData.Time);
			}

			UsersavesData.Add(FileName, NewData);
		}

		pGameInstance->OtherSettings->GetValueAsInteger("SlotSave", CurrentSlot);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("m_bLoadData == false"));
	}

	m_bLoadData = true;
}

void USaveSystem::UnLoadData()
{
	m_bLoadData = false;
	UsersavesData.Empty();
	AutosavesData.Empty();
}

bool USaveSystem::IsLoadData() const
{
	return m_bLoadData;
}

int32 USaveSystem::GetCurSlot()
{
	return CurrentSlot;
}

void USaveSystem::SetCurSlot(int32 NewSlot)
{
	CurrentSlot = NewSlot;
}

void USaveSystem::ChangeSaveSlot()
{
	CurrentSlot = CurrentSlot + 1;
	if (CurrentSlot >= 3) {
		CurrentSlot = 0;
	}
}

void USaveSystem::ClearCurrentSlot()
{
	FString SaveName = FString::Printf(TEXT("%s%d"), *AutosaveName, CurrentSlot);
	AutosavesData.Remove(SaveName);

	FString FullPath = FString::Printf(TEXT("%s/SaveGames/%s.sav"), *FPaths::ProjectSavedDir(), *SaveName);
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FullPath);

	pGameInstance->SaveGameParams();
	pGameInstance->SaveOtherSettings();
}

void USaveSystem::LoadSlotFile(FString FileName)
{
	m_oSlotData = GetInfoSaveFile(FileName);
	UGameplayStatics::OpenLevel(this, FName(m_oSlotData.LevelName.Replace(
		*GetWorld()->StreamingLevelsPrefix, TEXT(""))));
}
