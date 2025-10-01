// (c) XenFFly

#include "ShatalovSavesUtils.h"

bool UShatalovSavesUtils::SaveStringToFile(const FString& SaveDirectory, const FString& FileName, const FString& SaveString)
{
	return FFileHelper::SaveStringToFile(SaveString, *(SaveDirectory / FileName));
}