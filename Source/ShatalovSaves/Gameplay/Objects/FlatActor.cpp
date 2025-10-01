// (c) XenFFly

#include "FlatActor.h"
#include "../../Framework/JSON.h"

AFlatActor::AFlatActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlatActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFlatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlatActor::Save(class UJSONLibrary* JsonObject)
{
    ClassGeneratedBy = TSoftClassPtr<AActor>(GetClass());
    FProperty* PropClass = FindFProperty(AFlatActor, ClassGeneratedBy);
    JsonObject->SetValueAsWildcard("ClassGeneratedBy", PropClass, &ClassGeneratedBy);

    JsonObject->SetValueAsTransform("Transform", GetActorTransform());

    /* Save FlatComponent
    * 
    FlatTypeLoaded = FlatComponent->Type;
    FProperty* PropFT = FindFProperty(AFlatActor, FlatTypeLoaded);
    JsonObject->SetValueAsWildcard("Type", PropFT, &FlatTypeLoaded);

    FlatModeLoaded = FlatComponent->Mode;
    FProperty* PropFM = FindFProperty(AFlatActor, FlatModeLoaded);
    JsonObject->SetValueAsWildcard("Mode", PropFM, &FlatModeLoaded);
    */

    OnSave(JsonObject);
}

void AFlatActor::Load(UJSONLibrary* JsonObject)
{
    /* Load FlatComponent
   *
   FProperty* PropFT = FindFProperty(AFlatActor, FlatTypeLoaded);
   JsonObject->GetValueAsWildcard("Type", PropFT, &FlatTypeLoaded);
   FlatComponent->SetFlatType(FlatTypeLoaded);

   FProperty* PropFM = FindFProperty(AFlatActor, FlatModeLoaded);
   JsonObject->SetValueAsWildcard("Mode", PropFM, &FlatModeLoaded);
   FlatComponent->SetFlatMode(FlatModeLoaded);
    */
    
    FTransform LoadedTransform;
    JsonObject->GetValueAsTransform("Transform", LoadedTransform);

    /*if (FlatTypeLoaded == FT_Static)*/
        SetActorTransform(LoadedTransform);

	OnLoad(JsonObject);
}

