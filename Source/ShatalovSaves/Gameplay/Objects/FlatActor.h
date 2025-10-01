// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlatActor.generated.h"

UCLASS()
class SHATALOVSAVES_API AFlatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlatActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called before the game save process.
	UFUNCTION(BlueprintNativeEvent)
	void OnPreSave();
	virtual void OnPreSave_Implementation() {};

	// Save game process.
	UFUNCTION(BlueprintNativeEvent)
	void OnSave(class UJSONLibrary* JsonObject);
	virtual void OnSave_Implementation(class UJSONLibrary* JsonObject) {};
	virtual void Save(class UJSONLibrary* JsonObject);

	// Load game process.
	UFUNCTION(BlueprintNativeEvent)
	void OnLoad(class UJSONLibrary* JsonObject);
	virtual void OnLoad_Implementation(class UJSONLibrary* JsonObject) {};
	virtual void Load(class UJSONLibrary* JsonObject);


	// Works like Begin Play, but after loading a save game.
	UFUNCTION(BlueprintNativeEvent)
	void OnPostLoad();
	virtual void OnPostLoad_Implementation() {};

private:
	/* Save FlatComponent
	* 
	UPROPERTY()
	TEnumAsByte<EFlatType> FlatTypeLoaded;
	UPROPERTY()
	TEnumAsByte<EFlatMode> FlatModeLoaded;
	UPROPERTY()
	*/
	TSoftClassPtr<AFlatActor> ClassGeneratedBy;
};
