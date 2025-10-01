// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HelloNeighborRebornGameInstance.generated.h"

UCLASS(DisplayName="HNCODE GameInstance")
class SHATALOVSAVES_API UHelloNeighborRebornGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;
public:
	// Saves Key
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveSystem")
	FString EncryptionJsonKey;

	// Every n seconds changes save slot from 1 to 3.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveSystem")
	float TimeAutosave = 600.f;

// HN Vibes
public:
	// gp.sav
	void SaveGameParams();
	void LoadGameParams();
	// os.sav
	void SaveOtherSettings();
	void LoadOtherSettings();

	UFUNCTION(BlueprintPure)
	class USaveSystem* GetSaveSystem() const; 

	UFUNCTION(BlueprintCallable)
	void SaveSettingsGame();
	
	UPROPERTY()
	class UJSONLibrary* OtherSettings;

	UPROPERTY()
	class UJSONLibrary* GameParams;

protected:
	UPROPERTY()
	class USaveSystem* SaveSystem;
};
