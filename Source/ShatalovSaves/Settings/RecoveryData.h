// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RecoveryData.generated.h"

enum ESaveOwner : int;

USTRUCT(BlueprintType)
struct FSaveParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSaveObjects = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSavePlayer = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSaveNeighbor = true;

	int32 SelectedSlot = -1;

	FSaveParams()
	{
	}

	FSaveParams(bool bIsSaveObjects, bool bInSavePlayer, bool bInSaveNeighbor)
	{
		bSaveObjects = bIsSaveObjects;
		bSavePlayer = bInSavePlayer;
		bSaveNeighbor = bInSaveNeighbor;
	}

	FSaveParams(int32 InSelectedSlot)
	{
		SelectedSlot = InSelectedSlot;
	}
};

UCLASS()
class SHATALOVSAVES_API URecoveryData : public UObject
{
	GENERATED_BODY()

public:
	void Init(class AHelloNeighborRebornGameMode* GameMode);
	virtual void PostLoad() override;
	
	void ClearSaveData();
	bool IsLoadObjects() const;
	void SetLoadObjects(bool bLoad);
	
	void LoadObjects();
	void Save(FString LevelName, FSaveParams Params, ESaveOwner Type);

	void DeleteSaveFile();

	bool bIsLoadData;

private:
	// DEPRECATED
	bool LoadFile(FString Filename, UWorld* World);

	class UHelloNeighborRebornGameInstance* pGameInstance;
	class AHelloNeighborRebornGameMode* pGameMode;
	
};
