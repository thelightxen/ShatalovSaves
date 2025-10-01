// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Settings/RecoveryData.h"
#include "HelloNeighborRebornGameMode.generated.h"

UCLASS()
class SHATALOVSAVES_API AHelloNeighborRebornGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Make slot save file (sf_0, sf_1 and etc.)
	UFUNCTION(BlueprintCallable)
	void SaveGame(int32 Slot);

	// Make Autosave (autosave1)
	UFUNCTION(BlueprintCallable)
	void SaveLevel(FSaveParams SaveParams);

	// DEPRECATED, USE GetSaveSystem()->LoadSlotFile(FString)
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction))
	bool LoadSaveFile(FString Filename);

	// DEPRECATED
	void LoadSaveData();

	// if bSave == false, game can't be saved.
	UFUNCTION(BlueprintCallable)
	void SaveCurrentLevel(bool bSave);

	UFUNCTION(BlueprintPure)
	bool IsSaveCurrentLevel() const { return bSaveCurrentLevel; };

	UPROPERTY()
	class URecoveryData* RecoveryData;

private:
	UPROPERTY()
	class UHelloNeighborRebornGameInstance* m_pGameInstance;

	bool bSaveCurrentLevel = true;
};
