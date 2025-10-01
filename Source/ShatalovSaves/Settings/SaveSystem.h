// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SaveSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGame, bool, Save);

enum ESaveOwner
{
	Auto,
	User
};

USTRUCT(BlueprintType)
struct FSlotData 
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	int32 Version;

	UPROPERTY(BlueprintReadWrite)
	FString FileName;

	UPROPERTY(BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(BlueprintReadWrite)
	FString Date;

	UPROPERTY(BlueprintReadWrite)
	FString Time;

	UPROPERTY(BlueprintReadWrite)
	bool bAutosave;

	FSlotData()
		: FileName(TEXT("")), LevelName(TEXT("")), Date(TEXT("")), Time(TEXT("")), bAutosave(false)
	{
	}

	FSlotData(FString InFileName, FString InLevelName, FString InDate, FString InTime, bool bInAutosave)
	{
		FileName = InFileName;
		LevelName = InLevelName;
		Date = InDate;
		Time = InTime;
		bAutosave = bInAutosave;
	}

	FDateTime GetDateTime() const
	{
		FString DateTimeString = FString::Printf(TEXT("%s %s"), *Date, *Time);

		TArray<FString> DateTimeParts;
		DateTimeString.ParseIntoArray(DateTimeParts, TEXT(" "), true);

		TArray<FString> DateParts;
		DateTimeParts[0].ParseIntoArray(DateParts, TEXT("."), true);

		int32 Day = FCString::Atoi(*DateParts[0]);
		int32 Month = FCString::Atoi(*DateParts[1]);
		int32 Year = FCString::Atoi(*DateParts[2]);

		TArray<FString> TimeParts;
		DateTimeParts[1].ParseIntoArray(TimeParts, TEXT(":"), true);

		int32 Hour = FCString::Atoi(*TimeParts[0]);
		int32 Minute = FCString::Atoi(*TimeParts[1]);
		int32 Second = FCString::Atoi(*TimeParts[2]);

		FDateTime ParsedDateTime = FDateTime(Year, Month, Day, Hour, Minute, Second);

		return ParsedDateTime;
	}

};

UCLASS()
class SHATALOVSAVES_API USaveSystem : public UObject
{
	GENERATED_BODY()

public:
	void Init(class UHelloNeighborRebornGameInstance* GameInstance);
	
	static bool SaveJsonGameFile(class UHelloNeighborRebornGameInstance* GameInstance, const FString& Path,
	                      class TSharedPtr<FJsonObject> JsonObject);
	static bool LoadJsonGameFile(class UHelloNeighborRebornGameInstance* GameInstance, const FString& Path,
						  class TSharedPtr<FJsonObject>& JsonObject);

	bool DecryptGameFile(const FString& SaveName, TSharedPtr<FJsonObject>& JsonObject);
	bool Save(ESaveOwner Type, int Slot, const TSharedPtr<FJsonObject>& JsonObject);

	bool DownloadLastSave(TSharedPtr<FJsonObject>& JsonObject);

	void LoadData();
	void UnLoadData();
	bool IsLoadData() const;

	int32 GetCurSlot();
	void SetCurSlot(int32 Slot);
	void ChangeSaveSlot();

	void ClearCurrentSlot();
	
	UPROPERTY()
	TArray<TSoftObjectPtr<class AFlatActor>> DestroyedFlatActors;
public:
	
	UFUNCTION(BlueprintPure)
	TArray<FSlotData> GetAllSaves(bool bIncludeAutoSave, bool bSort = true, bool bCheckExists = true);
	
	UFUNCTION(BlueprintPure)
	int32 GetSaveCount() const;

	// autosave1, sf_4
	UFUNCTION(BlueprintPure)
	FSlotData GetInfoSaveFile(FString SaveName);

	UFUNCTION(BlueprintPure)
	int32 GetMaxSaves() { return 1000; };

	// autosave1, sf_4
	UFUNCTION(BlueprintCallable)
	void LoadSlotFile(FString FileName);

	// autosave1, sf_4
	UFUNCTION(BlueprintCallable)
	void DeleteSave(FString SaveName);

	UFUNCTION(BlueprintCallable, meta=(DevelopmentOnly))
	bool DecryptGameFileAsString(const FString& SaveName, FString& JsonString);

	UPROPERTY(BlueprintAssignable)
	FOnSaveGame OnSaveGame;

	UPROPERTY()
	FSlotData m_oSlotData;

private:
	UHelloNeighborRebornGameInstance* pGameInstance;

	FString AutosaveName = "autosave";
	FString UsersaveName = "sf_";

	// Filename/SlotData
	UPROPERTY()
	TMap<FString, FSlotData> AutosavesData;
	UPROPERTY()
	TMap<FString, FSlotData> UsersavesData;

	UPROPERTY()
	int32 SaveCount;
	
	UPROPERTY()
	bool m_bLoadData;

	UPROPERTY()
	int32 CurrentSlot;

	UPROPERTY()
	FTimerHandle ChangeSlotTimer;
};
