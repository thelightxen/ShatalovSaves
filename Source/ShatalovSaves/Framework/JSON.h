// (c) XenFFly

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonObjectConverter.h"
#include "JSON.generated.h"

#define FindFProperty(ClassName, VariableName) ClassName::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ClassName, VariableName));

DECLARE_DYNAMIC_DELEGATE_OneParam(FUploadSaveState, bool, bSuccess);

DECLARE_LOG_CATEGORY_EXTERN(LogShatalovSaves, Log, All);

UCLASS(Blueprintable, DisplayName="JSON", meta=(ScriptName="JSON"))
class SHATALOVSAVES_API UJSONLibrary : public UObject
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> JsonParsed = MakeShareable(new FJsonObject);

private:
	UPROPERTY()
		TArray<FTransform> Transform;
	UPROPERTY()
		TArray<FVector> Vector;
	UPROPERTY()
		TArray<FRotator> Rotator;

public:
	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsBool(FString FieldName, bool& BoolValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsBool(FString FieldName, bool BoolValue);


	UFUNCTION(BlueprintPure)
		bool GetValueAsString(FString FieldName, FString& StringValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsString(FString FieldName, FString StringValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsVector(FString FieldName, FVector& VectorValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsVector(FString FieldName, FVector VectorValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsRotator(FString FieldName, FRotator& RotatorValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsRotator(FString FieldName, FRotator RotatorValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsTransform(FString FieldName, FTransform& TransformValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsTransform(FString FieldName, FTransform TransformValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsFloat(FString FieldName, float& FloatValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsFloat(FString FieldName, float FloatValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsInteger(FString FieldName, int32& IntegerValue);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsInteger(FString FieldName, int32 IntegerValue);


	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsInteger64(FString FieldName, int64& Integer64Value);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool SetValueAsInteger64(FString FieldName, int64 Integer64Value);

	/*
	* Creates an object that defined as an array.
	* Создает объект, определенный как массив.
	*/ 
	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool CreateObjectValue(FString FieldName, UJSONLibrary*& ObjectValue);
	
	/*
	* Serves as a support for accurately retrieving data from an object array.
	* Служит поддержкой для точного извлечения данных из объекта-массива.
	*/ 
	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsObject(FString FieldName, UJSONLibrary*& ObjectValue);
	/*
	* Serves to simple retrieving data from an object array.
	* Служит для простого получения данных из объекта-массива.
	*/
	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetValueAsObjects(FString FieldName, TArray<UJSONLibrary*>& ObjectsValue);
	/*
	* Get object by name from an object array. Use with GetValueAsObject().
	* Получить объект по имени из объекта-массива. Используйте с GetValueAsObject().
	*/
	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		bool GetObjectByKey(const FString& FieldName, UJSONLibrary*& ReturnObject);
	/*
	* Get name of an object from an object array. Use with GetValueAsObject().
	* Получить имя объекта из объекта-массива. Используйте с GetValueAsObject().
	*/
	UFUNCTION(BlueprintPure, category = "HNCODE|JSON")
		FString GetRootFieldName(UJSONLibrary* ChildJsonObject);
	/*
	* Get object names of an object array. Use with GetValueAsObject().
	* Получить имена объектов из объекта-массива. Используйте с GetValueAsObject().
	*/
	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool GetFieldNames(TArray<FString>& FieldNames);
	/*
	* Get objects num in object
	* Получить кол-во объектов внутри объекта.
	*/
	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		int32 GetObjectsNum();

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool RemoveField(FString FieldName);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON")
		bool HasField(FString FieldName);

	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsStringArray(FString FieldName, TArray<FString> StringArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsStringArray(FString FieldName, TArray<FString>& StringArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsVectorArray(FString FieldName, TArray<FVector> VectorArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsVectorArray(FString FieldName, TArray<FVector>& VectorArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsRotatorArray(FString FieldName, TArray<FRotator> RotatorArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsRotatorArray(FString FieldName, TArray<FRotator>& RotatorArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsTransformArray(FString FieldName, TArray<FTransform> TransformArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsTransformArray(FString FieldName, TArray<FTransform>& TransformArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsFloatArray(FString FieldName, TArray<float> FloatArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsFloatArray(FString FieldName, TArray<float>& FloatArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsIntegerArray(FString FieldName, TArray<int32> IntegerArray);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsIntegerArray(FString FieldName, TArray<int32>& IntegerArray);


	UFUNCTION(BlueprintCallable, category = "HNCODE|JSON|Array")
		bool SetValueAsInteger64Array(FString FieldName, TArray<int64> Integer64Array);

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Array")
		bool GetValueAsInteger64Array(FString FieldName, TArray<int64>& Integer64Array);


	// Variable must be defined as UPROPERTY in header file. Use: FindFProperty(AMyActor, MyVariable);
	void SetValueAsWildcard(FString FieldName, FProperty* Property, void* ValuePtr)
	{
		if (Property)
		{
			JsonParsed->SetField(FieldName, UJSONLibrary::SetupValueWildcard(Property, ValuePtr));
		}
		else
		{
			ValuePtr = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Property not found."));
		}
	};

	// A Universal node.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value", DisplayName="Set Value as Wildcard"), category = "HNCODE|JSON|Universal")
		void K2_SetValueAsWildcard(FString FieldName, const UStruct* Value) {};
		DECLARE_FUNCTION(execK2_SetValueAsWildcard)
		{
			UJSONLibrary* Target = (UJSONLibrary*)P_THIS_OBJECT;

			P_GET_STRUCT_REF(FString, FieldName);

			Stack.Step(Stack.Object, NULL);

			FProperty* Prop = Stack.MostRecentProperty;
			void* Ptr = Stack.MostRecentPropertyAddress;

			P_FINISH;

			Target->JsonParsed->SetField(FieldName, UJSONLibrary::SetupValueWildcard(Prop, Ptr));
		}


	// Variable must be defined as UPROPERTY in header file. Use: FindFProperty(AMyActor, MyVariable);
	void GetValueAsWildcard(FString FieldName, FProperty* Property, void* ValuePtr)
	{
		if (this)
		{
			TSharedRef<FJsonValue> InJsonValue = UJSONLibrary::ReceiveValueWildcard(Property, JsonParsed->TryGetField(FieldName));

			if (InJsonValue->Type != EJson::None && InJsonValue->Type != EJson::Null)
			{
				FJsonObjectConverter::JsonValueToUProperty(InJsonValue, Property, ValuePtr, 0, 0);
			}
			else ValuePtr = nullptr;
		}
	};
	
	// A Universal node.
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "Value", DisplayName = "Get Value as Wildcard"), category = "HNCODE|JSON|Universal")
	void K2_GetValueAsWildcard(FString FieldName, UStruct*& Value);
	DECLARE_FUNCTION(execK2_GetValueAsWildcard)
	{
		UJSONLibrary* Target = (UJSONLibrary*)P_THIS_OBJECT;
		P_GET_STRUCT_REF(FString, FieldName);

		Stack.Step(Stack.Object, NULL);

		FProperty* Prop = Stack.MostRecentProperty;
		void* Ptr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		TSharedRef<FJsonValue> InJsonValue = UJSONLibrary::ReceiveValueWildcard(Prop, Target->JsonParsed->TryGetField(FieldName));

		if (InJsonValue->Type != EJson::None && InJsonValue->Type != EJson::Null)
		{
			FJsonObjectConverter::JsonValueToUProperty(InJsonValue, Prop, Ptr, 0, 0);
		}
	}
	// Resolves a FString into Soft Reference.
	UFUNCTION(BlueprintPure, category="HNCODE|JSON|Utils", meta=(CompactNodeTitle = "->"))
		static TSoftObjectPtr<UObject> ResolveStringSoftReference(const FString& String) 
		{
			FSoftObjectPath SoftRef;
			SoftRef.SetPath(String);

			return SoftRef.ResolveObject();
		}

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Utils", meta = (CompactNodeTitle = "->"))
		static TArray<TSoftObjectPtr<UObject>> ObjectArrayToSoftReferenceArray(TArray<UObject*> In) 
		{
			TArray<TSoftObjectPtr<UObject>> SoftArray;

			for (UObject* Object : In)
			{
				SoftArray.Add(TSoftObjectPtr<UObject>(Object));
			}
			return SoftArray;
		}

	UFUNCTION(BlueprintPure, category = "HNCODE|JSON|Utils", meta = (CustomStructureParam = "Out", CompactNodeTitle = "->"))
		static void SoftReferenceArrayToObjectArray(TArray<TSoftObjectPtr<UObject>> In, TArray<UStruct*>& Out) 
		{
			TArray<UStruct*> ObjectArray;

			for (TSoftObjectPtr<UObject> Soft : In)
			{
				ObjectArray.Add((UStruct*)Soft.Get());
			}
			Out = ObjectArray;
		}

private:

	static TSharedRef<FJsonValue> SetupValueWildcard(FProperty* Property, void* ValuePtr);
	static TSharedRef<FJsonValue> ReceiveValueWildcard(FProperty* Property, TSharedPtr<FJsonValue> Value);

};