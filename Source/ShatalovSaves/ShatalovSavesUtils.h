// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ShatalovSavesUtils.generated.h"

UCLASS()
class SHATALOVSAVES_API UShatalovSavesUtils : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool SaveStringToFile(const FString& SaveDirectory, const FString& FileName, const FString& SaveString);

};
