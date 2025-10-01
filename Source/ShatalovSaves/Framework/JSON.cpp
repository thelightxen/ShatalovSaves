// (c) XenFFly

#include "JSON.h"


DEFINE_LOG_CATEGORY(LogShatalovSaves);

bool UJSONLibrary::GetValueAsBool(FString FieldName, bool& BoolValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        return JsonParsed->TryGetBoolField(FieldName, BoolValue);
    }
    else return false;
}

bool UJSONLibrary::SetValueAsBool(FString FieldName, bool BoolValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TSharedPtr<FJsonValue> Value;
        JsonParsed->SetBoolField(FieldName, BoolValue);
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsString(FString FieldName, FString& StringValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        return JsonParsed->TryGetStringField(FieldName, StringValue);
    }
    else return false;
}

bool UJSONLibrary::SetValueAsString(FString FieldName, FString StringValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        JsonParsed->SetStringField(FieldName, StringValue);
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsVector(FString FieldName, FVector& VectorValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        double VectorX, VectorY, VectorZ;

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* VectorObject;
        bool Result = JsonParsed->TryGetObjectField(FieldName, VectorObject);

        if (!Result) return false;

        (*VectorObject)->TryGetNumberField("X", VectorX);
        (*VectorObject)->TryGetNumberField("Y", VectorY);
        (*VectorObject)->TryGetNumberField("Z", VectorZ);

        VectorValue = FVector(VectorX, VectorY, VectorZ);
        return true;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsVector(FString FieldName, FVector VectorValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TSharedPtr<FJsonObject> VectorObject = MakeShareable(new FJsonObject);
        VectorObject->SetNumberField("X", VectorValue.X);
        VectorObject->SetNumberField("Y", VectorValue.Y);
        VectorObject->SetNumberField("Z", VectorValue.Z);

        JsonParsed->SetObjectField(FieldName, VectorObject);
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsRotator(FString FieldName, FRotator& RotatorValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        double RotationX, RotationY, RotationZ;

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* RotatorObject;
        bool Result = JsonParsed->TryGetObjectField(FieldName, RotatorObject);

        if (!Result) return false;

        (*RotatorObject)->TryGetNumberField("Pitch", RotationX);
        (*RotatorObject)->TryGetNumberField("Yaw", RotationY);
        (*RotatorObject)->TryGetNumberField("Roll", RotationZ);

        RotatorValue = FRotator(RotationX, RotationY, RotationZ);
        return true;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsRotator(FString FieldName, FRotator RotatorValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TSharedPtr<FJsonObject> RotatorObject = MakeShareable(new FJsonObject);
        RotatorObject->SetNumberField("Pitch", RotatorValue.Pitch);
        RotatorObject->SetNumberField("Yaw", RotatorValue.Yaw);
        RotatorObject->SetNumberField("Roll", RotatorValue.Roll);

        JsonParsed->SetObjectField(FieldName, RotatorObject);
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsTransform(FString FieldName, FTransform& TransformValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        double RotationX, RotationY, RotationZ, RotationW;
        double TranslationX, TranslationY, TranslationZ;
        double Scale3DX, Scale3DY, Scale3DZ;

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* TransformObject;
        bool Result = JsonParsed->TryGetObjectField(FieldName, TransformObject);

        if (!Result) return false;
        
        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* RotationObject;
        (*TransformObject)->TryGetObjectField("Rotation", RotationObject);

        (*RotationObject)->TryGetNumberField("X", RotationX);
        (*RotationObject)->TryGetNumberField("Y", RotationY);
        (*RotationObject)->TryGetNumberField("Z", RotationZ);
        (*RotationObject)->TryGetNumberField("W", RotationW);

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* TranslationObject;
        (*TransformObject)->TryGetObjectField("Translation", TranslationObject);

        (*TranslationObject)->TryGetNumberField("X", TranslationX);
        (*TranslationObject)->TryGetNumberField("Y", TranslationY);
        (*TranslationObject)->TryGetNumberField("Z", TranslationZ);

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* Scale3DObject;
        (*TransformObject)->TryGetObjectField("Scale3D", Scale3DObject);

        (*Scale3DObject)->TryGetNumberField("X", Scale3DX);
        (*Scale3DObject)->TryGetNumberField("Y", Scale3DY);
        (*Scale3DObject)->TryGetNumberField("Z", Scale3DZ);

        TransformValue = 
            FTransform(
                FQuat(RotationX, RotationY, RotationZ, RotationW), 
                FVector(TranslationX, TranslationY, TranslationZ), 
                FVector(Scale3DX, Scale3DY, Scale3DZ)
            );
        return true;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsTransform(FString FieldName, FTransform TransformValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TSharedPtr<FJsonObject> TransformObject = MakeShareable(new FJsonObject);
        
        // Translation
        TSharedPtr<FJsonObject> TranslationObject = MakeShareable(new FJsonObject);
        TranslationObject->SetNumberField("X", TransformValue.GetTranslation().X);
        TranslationObject->SetNumberField("Y", TransformValue.GetTranslation().Y);
        TranslationObject->SetNumberField("Z", TransformValue.GetTranslation().Z);
        TransformObject->SetObjectField("Translation", TranslationObject);

        // Rotation
        FQuat Rotation;
        TSharedPtr<FJsonObject> RotationObject = MakeShareable(new FJsonObject);
        RotationObject->SetNumberField("X", TransformValue.GetRotation().X);
        RotationObject->SetNumberField("Y", TransformValue.GetRotation().Y);
        RotationObject->SetNumberField("Z", TransformValue.GetRotation().Z);
        RotationObject->SetNumberField("W", TransformValue.GetRotation().W);
        TransformObject->SetObjectField("Rotation", RotationObject);

        // Scale3D
        TSharedPtr<FJsonObject> Scale3DObject = MakeShareable(new FJsonObject);
        Scale3DObject->SetNumberField("X", TransformValue.GetScale3D().X);
        Scale3DObject->SetNumberField("Y", TransformValue.GetScale3D().Y);
        Scale3DObject->SetNumberField("Z", TransformValue.GetScale3D().Z);
        TransformObject->SetObjectField("Scale3D", Scale3DObject);

        JsonParsed->SetObjectField(FieldName, TransformObject); 
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsFloat(FString FieldName, float& FloatValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        double FloatDouble;
        bool Result = JsonParsed->TryGetNumberField(FieldName, FloatDouble);
        FloatValue = (float) FloatDouble;
	    return Result;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsFloat(FString FieldName, float FloatValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        JsonParsed->SetNumberField(FieldName, FloatValue);
        return true;
    }
    else return false;
}


bool UJSONLibrary::GetValueAsInteger(FString FieldName, int32& IntegerValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        return JsonParsed->TryGetNumberField(FieldName, IntegerValue);
    }
    else return false;
}

bool UJSONLibrary::SetValueAsInteger(FString FieldName, int32 IntegerValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        JsonParsed->SetNumberField(FieldName, IntegerValue);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsInteger64(FString FieldName, int64& Integer64Value)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        return JsonParsed->TryGetNumberField(FieldName, Integer64Value);
    }
    else return false;
}

bool UJSONLibrary::SetValueAsInteger64(FString FieldName, int64 Integer64Value)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        JsonParsed->SetNumberField(FieldName, Integer64Value);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsObject(FString FieldName, UJSONLibrary*& ObjectValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TSharedPtr<FJsonObject>* JsonParsedPtr;
        bool Resp = JsonParsed->TryGetObjectField(FieldName, JsonParsedPtr);

        if (Resp) 
        {

            ObjectValue = NewObject<UJSONLibrary>();

            ObjectValue->JsonParsed = *JsonParsedPtr;

            return true;
        }

        ObjectValue = nullptr;
        return false;
    }
    ObjectValue = nullptr;
    return false;
}

bool UJSONLibrary::HasField(FString FieldName)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        return JsonParsed->HasField(FieldName);
    }
    return false;
}

bool UJSONLibrary::GetValueAsObjects(FString FieldName, TArray<UJSONLibrary*>& ObjectsValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TSharedPtr<FJsonObject>* RootObject;
        if (JsonParsed->TryGetObjectField(FieldName, RootObject))
        {
            for (auto& Pair : (*RootObject)->Values)
            {
                UJSONLibrary* ObjectJSON = NewObject<UJSONLibrary>();
                ObjectJSON->JsonParsed = Pair.Value->AsObject();
                ObjectsValue.Add(ObjectJSON);
            }
            return true;
        }
        else return false;
    }
    else return false;
}

TSharedRef<FJsonValue> UJSONLibrary::SetupValueWildcard(FProperty* Property, void* ValuePtr)
{
    if (ValuePtr == NULL || Property == NULL)
    {
        return MakeShareable(new FJsonValueNull());
    }
    else if (CastField<FArrayProperty>(Property))
    {
        FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property);
        TArray<TSharedPtr<FJsonValue>> Array;

        auto Helper = FScriptArrayHelper::CreateHelperFormInnerProperty(ArrayProp->Inner, ValuePtr);
        for (int32 i = 0; i < Helper.Num(); i++)
        {
            Array.Add(SetupValueWildcard(ArrayProp->Inner, Helper.GetRawPtr(i)));
        }

        return MakeShareable(new FJsonValueArray(Array));
    }
    else if (CastField<FSetProperty>(Property))
    {
        FSetProperty* SetProp = CastField<FSetProperty>(Property);
        TArray<TSharedPtr<FJsonValue>> Array;

        auto Helper = FScriptSetHelper::CreateHelperFormElementProperty(SetProp->ElementProp, ValuePtr);
        for (int32 i = 0; i < Helper.Num(); i++)
        {
            Array.Add(SetupValueWildcard(SetProp->ElementProp, Helper.GetElementPtr(i)));
        }

        return MakeShareable(new FJsonValueArray(Array));
    }
    else if (CastField<FMapProperty>(Property))
    {
        FMapProperty* MapProp = CastField<FMapProperty>(Property);
        TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

        auto Helper = FScriptMapHelper::CreateHelperFormInnerProperties(MapProp->KeyProp, MapProp->ValueProp, ValuePtr);
        for (int32 i = 0; i < Helper.Num(); i++)
        {
            FString KeyString;
            if (!SetupValueWildcard(MapProp->KeyProp, Helper.GetKeyPtr(i))->TryGetString(KeyString))
            {
                MapProp->KeyProp->ExportTextItem(KeyString, Helper.GetKeyPtr(i), nullptr, nullptr, 0);
            }

            JsonObject->SetField(KeyString, SetupValueWildcard(MapProp->ValueProp, Helper.GetValuePtr(i)));
        }

        return MakeShareable(new FJsonValueObject(JsonObject));
    }
    else if (CastField<FStructProperty>(Property))
    {
        FStructProperty* StructProp = CastField<FStructProperty>(Property);
        TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
        for (TFieldIterator<FProperty> Iterator(StructProp->Struct); Iterator; ++Iterator)
        {
            FProperty* Prop = *Iterator;
            JsonObject->SetField(Prop->GetAuthoredName(), SetupValueWildcard(Prop, Prop->ContainerPtrToValuePtr<void*>(ValuePtr)));
        }

        return MakeShareable(new FJsonValueObject(JsonObject));
    }
    else if (CastField<FObjectProperty>(Property))
    {
        FObjectProperty* ObjProp = CastField<FObjectProperty>(Property);
        void* PropVal = ObjProp->GetObjectPropertyValue(ValuePtr);
        if (PropVal == NULL) return MakeShareable(new FJsonValueNull());

        if (ObjProp->PropertyClass->IsNative())
        {
            return FJsonObjectConverter::UPropertyToJsonValue(Property, ValuePtr, 0, 0).ToSharedRef();
        }

        TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
        for (TFieldIterator<FProperty> Iterator(ObjProp->PropertyClass); Iterator; ++Iterator)
        {
            FProperty* Prop = *Iterator;
            JsonObject->SetField(Prop->GetAuthoredName(), SetupValueWildcard(Prop, Prop->ContainerPtrToValuePtr<void*>(PropVal)));
        }

        return MakeShareable(new FJsonValueObject(JsonObject));
    }
    else
    {
        return  FJsonObjectConverter::UPropertyToJsonValue(Property, ValuePtr, 0, 0).ToSharedRef();
    }
}

TSharedRef<FJsonValue> UJSONLibrary::ReceiveValueWildcard(FProperty* Property, TSharedPtr<FJsonValue> Value)
{
    if (!Value.IsValid() || Value->IsNull() || Property == NULL)
    {
        return MakeShareable(new FJsonValueNull());
    }
    else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
    {
        TArray<TSharedPtr<FJsonValue>> OutArray;
        if (Value->Type == EJson::Array)
        {
            for (int32 ArrayIndex = 0; ArrayIndex < Value->AsArray().Num(); ArrayIndex++)
            {
                OutArray.Add(ReceiveValueWildcard(ArrayProp->Inner, Value->AsArray()[ArrayIndex]));
            }
        }

        return MakeShareable(new FJsonValueArray(OutArray));
    }
    else if (FSetProperty* SetProp = CastField<FSetProperty>(Property))
    {
        TArray<TSharedPtr<FJsonValue>> OutSet;
        if (Value->Type == EJson::Array)
        {
            for (int32 ArrayIndex = 0; ArrayIndex < Value->AsArray().Num(); ArrayIndex++)
            {
                OutSet.Add(ReceiveValueWildcard(SetProp->ElementProp, Value->AsArray()[ArrayIndex]));
            }
        }
        
        return MakeShareable(new FJsonValueArray(OutSet));
    }
    else if (FMapProperty* MapProp = CastField<FMapProperty>(Property))
    {
        TSharedRef<FJsonObject> OutMap = MakeShared<FJsonObject>();
        if (Value->Type == EJson::Object)
        {
            for (auto Val : Value->AsObject()->Values)
            {
                OutMap->SetField(Val.Key, ReceiveValueWildcard(MapProp->ValueProp, Val.Value));
            }
        }

        return MakeShareable(new FJsonValueObject(OutMap));
    }
    else if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
    {
        TSharedRef<FJsonObject> OutObject = MakeShared<FJsonObject>();
        if (Value->Type == EJson::Object)
        {
            for (TFieldIterator<FProperty> Iterator(StructProp->Struct); Iterator; ++Iterator)
            {
                FProperty* Prop = *Iterator;
                if (Value->AsObject()->HasField(Prop->GetAuthoredName()))
                {
                    OutObject->SetField(Prop->GetName(), ReceiveValueWildcard(Prop, Value->AsObject()->Values.Find(Prop->GetAuthoredName())->ToSharedRef()));
                }
                else if (Value->AsObject()->HasField(Property->GetName()))
                {
                    OutObject->SetField(Prop->GetName(), ReceiveValueWildcard(Prop, Value->AsObject()->Values.Find(Prop->GetName())->ToSharedRef()));
                }
            }
        }

        return MakeShareable(new FJsonValueObject(OutObject));
    }
    else if (FObjectProperty* ObjectProp = CastField<FObjectProperty>(Property))
    {
        TSharedRef<FJsonObject> OutObject = MakeShared<FJsonObject>();
        if (ObjectProp->PropertyClass->IsNative())
        {
            return Value.ToSharedRef();
        }

        if (Value->Type == EJson::Object)
        {
            for (TFieldIterator<FProperty> Iterator(ObjectProp->PropertyClass); Iterator; ++Iterator)
            {
                FProperty* Prop = *Iterator;
                if (Value->AsObject()->HasField(Prop->GetAuthoredName()))
                {
                    OutObject->SetField(Prop->GetName(), ReceiveValueWildcard(Prop, Value->AsObject()->Values.Find(Prop->GetAuthoredName())->ToSharedRef()));
                }
                else if (Value->AsObject()->HasField(Prop->GetName()))
                {
                    OutObject->SetField(Prop->GetName(), ReceiveValueWildcard(Prop, Value->AsObject()->Values.Find(Prop->GetName())->ToSharedRef()));
                }
            }
        }

        return MakeShareable(new FJsonValueObject(OutObject));
    }
    else return Value.ToSharedRef();
}

bool UJSONLibrary::GetObjectByKey(const FString& FieldName, UJSONLibrary*& ReturnObject)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TSharedPtr<FJsonObject>* SubObject;
        if (JsonParsed->TryGetObjectField(FieldName, SubObject))
        {
            ReturnObject = NewObject<UJSONLibrary>();
            ReturnObject->JsonParsed = *SubObject;
            return true;
        }
        ReturnObject = nullptr;
        return false;
    }
    ReturnObject = nullptr;
    return false;
}

FString UJSONLibrary::GetRootFieldName(UJSONLibrary* ChildJsonObject)
{
    if (IsValid(this) && JsonParsed.IsValid() && JsonParsed.IsValid())
    {
        for (const auto& Pair : JsonParsed->Values)
        {
            const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe> CurrentSubObject = Pair.Value->AsObject();
            if (CurrentSubObject && CurrentSubObject == ChildJsonObject->JsonParsed)
            {
                return Pair.Key;
            }
        }
    }

    return FString();
}

bool UJSONLibrary::CreateObjectValue(FString FieldName, UJSONLibrary*& ObjectValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TSharedPtr<FJsonObject> JsonEmpty = MakeShareable(new FJsonObject);
        JsonParsed->SetObjectField(FieldName, JsonEmpty);

        const TSharedPtr<FJsonObject, ESPMode::NotThreadSafe>* JsonParsedPtr;
        bool Resp = JsonParsed->TryGetObjectField(FieldName, JsonParsedPtr);

        if (Resp && JsonParsedPtr && *JsonParsedPtr)
        {
            ObjectValue = NewObject<UJSONLibrary>();
            ObjectValue->JsonParsed = *JsonParsedPtr;
            return true;
        }

        return false;
    }
    else return false;
}

bool UJSONLibrary::RemoveField(FString FieldName)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        JsonParsed->RemoveField(FieldName);
        return !JsonParsed->HasField(FieldName);
    }
    else return false;
}

bool UJSONLibrary::SetValueAsStringArray(FString FieldName, TArray<FString> StringArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> JsonValues;
        for (const FString& Value : StringArray)
        {
            TSharedPtr<FJsonValue> JsonString = MakeShareable(new FJsonValueString(Value));
            JsonValues.Add(JsonString);
        }
        
        JsonParsed->SetArrayField(FieldName, JsonValues);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsStringArray(FString FieldName, TArray<FString>& StringArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonValuesPtr = nullptr;
        if (JsonParsed->TryGetArrayField(FieldName, JsonValuesPtr))
        {
            StringArray.Empty();
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonValuesPtr)
            {
                if (JsonValue->Type == EJson::String)
                {
                    FString StringValue = JsonValue->AsString();
                    StringArray.Add(StringValue);
                }
            }

            return true;
        }
    }
    return false;
}


bool UJSONLibrary::GetValueAsVectorArray(FString FieldName, TArray<FVector>& VectorArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue, ESPMode::NotThreadSafe>>* VectorObjects;
        if (JsonParsed->TryGetArrayField(FieldName, VectorObjects))
        {
            for (const auto& VectorValue : *VectorObjects)
            {
                if (VectorValue.IsValid() && VectorValue->Type == EJson::Object)
                {
                    double VectorX, VectorY, VectorZ;
                    const TSharedPtr<FJsonObject> VectorObject = VectorValue->AsObject();

                    VectorObject->TryGetNumberField("X", VectorX);
                    VectorObject->TryGetNumberField("Y", VectorY);
                    VectorObject->TryGetNumberField("Z", VectorZ);

                    FVector NewVector(VectorX, VectorY, VectorZ);
                    VectorArray.Add(NewVector);
                }
            }
            return true;
        }
    }
    return false;
}

bool UJSONLibrary::SetValueAsVectorArray(FString FieldName, TArray<FVector> VectorArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        UJSONLibrary* JLib = NewObject<UJSONLibrary>();
        JLib->Vector = VectorArray;
        FProperty* Prop = JLib->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UJSONLibrary, Vector));
        SetValueAsWildcard(FieldName, Prop, &JLib->Vector);
        JLib = NULL;
        return true;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsRotatorArray(FString FieldName, TArray<FRotator> RotatorArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        UJSONLibrary* JLib = NewObject<UJSONLibrary>();
        JLib->Rotator = RotatorArray;
        FProperty* Prop = JLib->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UJSONLibrary, Rotator));
        SetValueAsWildcard(FieldName, Prop, &JLib->Rotator);
        JLib = NULL;
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsRotatorArray(FString FieldName, TArray<FRotator>& RotatorArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        UJSONLibrary* JLib = NewObject<UJSONLibrary>();
        JLib->Rotator = RotatorArray;
        FProperty* Prop = JLib->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UJSONLibrary, Rotator));
        GetValueAsWildcard(FieldName, Prop, &JLib->Rotator);
        JLib = NULL;
        return true;
    }
    else return false;
}


bool UJSONLibrary::SetValueAsTransformArray(FString FieldName, TArray<FTransform> TransformArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        UJSONLibrary* JLib = NewObject<UJSONLibrary>();
        JLib->Transform = TransformArray;
        FProperty* Prop = JLib->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UJSONLibrary, Transform));
        SetValueAsWildcard(FieldName, Prop, &JLib->Transform);
        JLib = NULL;
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsTransformArray(FString FieldName, TArray<FTransform>& TransformArray)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        UJSONLibrary* JLib = NewObject<UJSONLibrary>();
        JLib->Transform = TransformArray;
        FProperty* Prop = JLib->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UJSONLibrary, Transform));
        GetValueAsWildcard(FieldName, Prop, &JLib->Transform);
        JLib = NULL;
        return true;
    }
    else return false;
}

bool UJSONLibrary::SetValueAsFloatArray(FString FieldName, TArray<float> ArrayValue)
{
    if (this && JsonParsed.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> JsonValues;
        for (const float& Value : ArrayValue)
        {
            TSharedPtr<FJsonValue> JsonString = MakeShareable(new FJsonValueNumber(Value));
            JsonValues.Add(JsonString);
        }
        JsonParsed->SetArrayField(FieldName, JsonValues);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsFloatArray(FString FieldName, TArray<float>& ArrayValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonValuesPtr = nullptr;
        if (JsonParsed->TryGetArrayField(FieldName, JsonValuesPtr))
        {
            ArrayValue.Empty();
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonValuesPtr)
            {
                if (JsonValue->Type == EJson::Number)
                {
                    float FloatValue = JsonValue->AsNumber();
                    ArrayValue.Add(FloatValue);
                }
            }
            
            return true;
        }
    }
    return false;
}


bool UJSONLibrary::SetValueAsIntegerArray(FString FieldName, TArray<int32> ArrayValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> JsonValues;
        for (const int32& Value : ArrayValue)
        {
            TSharedPtr<FJsonValue> JsonString = MakeShareable(new FJsonValueNumber(Value));
            JsonValues.Add(JsonString);
        }

        JsonParsed->SetArrayField(FieldName, JsonValues);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsIntegerArray(FString FieldName, TArray<int32>& ArrayValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonValuesPtr = nullptr;
        if (JsonParsed->TryGetArrayField(FieldName, JsonValuesPtr))
        {
            ArrayValue.Empty();
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonValuesPtr)
            {
                if (JsonValue->Type == EJson::Number)
                {
                    float FloatValue = JsonValue->AsNumber();
                    ArrayValue.Add(FloatValue);
                }
            }
            return true;
        }
    }
    return false;
}


bool UJSONLibrary::SetValueAsInteger64Array(FString FieldName, TArray<int64> ArrayValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> JsonValues;
        for (const int64& Value : ArrayValue)
        {
            TSharedPtr<FJsonValue> JsonString = MakeShareable(new FJsonValueNumber(Value));
            JsonValues.Add(JsonString);
        }

        JsonParsed->SetArrayField(FieldName, JsonValues);
        return true;
    }
    else return false;
}

bool UJSONLibrary::GetValueAsInteger64Array(FString FieldName, TArray<int64>& ArrayValue)
{
    if (IsValid(this) && JsonParsed.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* JsonValuesPtr = nullptr;
        if (JsonParsed->TryGetArrayField(FieldName, JsonValuesPtr))
        {
            ArrayValue.Empty();
            for (const TSharedPtr<FJsonValue>& JsonValue : *JsonValuesPtr)
            {
                if (JsonValue->Type == EJson::Number)
                {
                    float FloatValue = JsonValue->AsNumber();
                    ArrayValue.Add(FloatValue);
                }
            }
            return true;
        }
    }
    return false;
}

bool UJSONLibrary::GetFieldNames(TArray<FString>& FieldNames)
{
    if (IsValid(this) && JsonParsed)
    {
        JsonParsed->Values.GetKeys(FieldNames);
        return true;
    }
    return false;
}

int32 UJSONLibrary::GetObjectsNum()
{
    if (IsValid(this) && JsonParsed)
    {
        return JsonParsed->Values.Num();
    }
    return 0;
}