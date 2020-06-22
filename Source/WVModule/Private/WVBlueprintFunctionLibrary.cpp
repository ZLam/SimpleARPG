// Fill out your copyright notice in the Description page of Project Settings.


#include "WVBlueprintFunctionLibrary.h"
#include "Logger/WVLog.h"
#include "ConfigUtil/WVConfigUtil.h"
#include "EventSys/WVEventDispatcher.h"

void UWVBlueprintFunctionLibrary::FuncParamsOfProperties(UFunction* inFunc, TArray<UProperty*> &outArr)
{
	if (!inFunc)
	{
		return;
	}
	TFieldIterator<UProperty> ite(inFunc);
	while ((ite) && (ite->PropertyFlags & CPF_Parm))
	{
		outArr.Add(*ite);
		++ite;
	}
}

void UWVBlueprintFunctionLibrary::LogI(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	FWVLog::GetInstance()->Info(strLog, bPrintToLog, bPrintToScreen, duration);
}

void UWVBlueprintFunctionLibrary::LogW(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	FWVLog::GetInstance()->Warning(strLog, bPrintToLog, bPrintToScreen, duration);
}

void UWVBlueprintFunctionLibrary::LogF(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	FWVLog::GetInstance()->Fatal(strLog, bPrintToLog, bPrintToScreen, duration);
}

void UWVBlueprintFunctionLibrary::LogE(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	FWVLog::GetInstance()->Error(strLog, bPrintToLog, bPrintToScreen, duration);
}

UDataTable* UWVBlueprintFunctionLibrary::GetConfig(EWVConfigName configName)
{
	return UWVConfigUtil::GetInstance()->GetConfig(configName);
}

FString UWVBlueprintFunctionLibrary::ConvToEventSignature(EWVEventCategory inCategory, EWVEventName inEventName)
{
	return UWVEventDispatcher::GetInstance()->GetEventSignature(inCategory, inEventName);
}

void UWVBlueprintFunctionLibrary::AddListener_FN(const FString& inEventSignature, UObject* inCaller,
                                                 const FString& inFuncName)
{
	UWVEventDispatcher::GetInstance()->AddListener(inEventSignature, inCaller, inFuncName);
}

void UWVBlueprintFunctionLibrary::AddListener_FN_SP(EWVEventCategory inCategory, EWVEventName inEventName,
	UObject* inCaller, const FString& inFuncName)
{
	UWVEventDispatcher::GetInstance()->AddListener(inCategory, inEventName, inCaller, inFuncName);
}

void UWVBlueprintFunctionLibrary::AddListener_DL(const FString& inEventSignature, UObject* inCaller,
	FWVEventDelegate_One inDelegateOne)
{
	UWVEventDispatcher::GetInstance()->AddListener(inEventSignature, inCaller, inDelegateOne);
}

void UWVBlueprintFunctionLibrary::AddListener_DL_SP(EWVEventCategory inCategory, EWVEventName inEventName,
	UObject* inCaller, FWVEventDelegate_One inDelegateOne)
{
	UWVEventDispatcher::GetInstance()->AddListener(inCategory, inEventName, inCaller, inDelegateOne);
}

void UWVBlueprintFunctionLibrary::FireEvent(const FString& inEventSignature)
{
	UWVEventDispatcher::GetInstance()->FireEvent(inEventSignature);
}

void UWVBlueprintFunctionLibrary::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName)
{
	FString eventSign = UWVEventDispatcher::GetInstance()->GetEventSignature(inCategory, inEventName);
	UWVEventDispatcher::GetInstance()->FireEvent(eventSign);
}

void UWVBlueprintFunctionLibrary::FireEvent_OneParams(const FString& inEventSignature, UProperty* params)
{
	//this never gets called due to custom thunk
}

void UWVBlueprintFunctionLibrary::FireEvent_OneParams_SP(EWVEventCategory inCategory, EWVEventName inEventName,
	UProperty* params)
{
	//this never gets called due to custom thunk
}

void UWVBlueprintFunctionLibrary::_FireEvent_OneParams(const FString& inEventSignature, FWVEventParams_BP& params)
{
	UWVEventDispatcher::GetInstance()->FireEvent_BP(inEventSignature, params);
}

void UWVBlueprintFunctionLibrary::_FireEvent_OneParams_SP(EWVEventCategory inCategory, EWVEventName inName,
	FWVEventParams_BP& params)
{
	FString eventSignature = UWVEventDispatcher::GetInstance()->GetEventSignature(inCategory, inName);
	UWVEventDispatcher::GetInstance()->FireEvent_BP(eventSignature, params);
}

float UWVBlueprintFunctionLibrary::GetAngleBetween2Vector(FVector v1, FVector v2)
{
	v1.Z = 0.0f;
	v2.Z = 0.0f;
	float dotVal = FVector::DotProduct(v1, v2);
	return FMath::RadiansToDegrees(FMath::Acos(dotVal));
}

/**
 * 0（重叠），1（在前面），2（在后面），3（在左边），4（在右边）
 */
int32 UWVBlueprintFunctionLibrary::GetRelativeDirection(AActor* srcActor, AActor* dstActor)
{
	FVector scrPos = srcActor->GetActorLocation();
	FVector dstPos = dstActor->GetActorLocation();

	scrPos.Z = 0.0f;
	dstPos.Z = 0.0f;
	
	if (scrPos.Equals(dstPos))
	{
		return 0;
	}
	
	FVector tRelativeDirection = dstPos - scrPos;
	float tDotVal = FVector::DotProduct(srcActor->GetActorForwardVector(), tRelativeDirection);

	if (FMath::IsNearlyEqual(tDotVal, 1.0f))
	{
		return 1;
	}
	if (FMath::IsNearlyEqual(tDotVal, -1.0f))
	{
		return 2;
	}

	FVector tNormal = FVector::CrossProduct(srcActor->GetActorForwardVector(), tRelativeDirection).GetSafeNormal();

	if (tNormal.Z < 0)
	{
		return 3;
	}
	else
	{
		return 4;
	}
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToBool(const FWVEventDelegateParams_One& delegateParams, bool& out)
{
	bool ret = false;
	if (IsValid(delegateParams.dataInfoPtr) && delegateParams.dataPtr)
	{
		if (delegateParams.dataInfoPtr->IsA<UBoolProperty>())
		{
			UBoolProperty *tProp = Cast<UBoolProperty>(delegateParams.dataInfoPtr);
			if (tProp)
			{
				bool val = tProp->GetPropertyValue(delegateParams.dataPtr);
				out = val;
				ret = true;
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
			}
		}
		else
		{
			WVLogW(TEXT("delegateParams not a bool"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToInt32(const FWVEventDelegateParams_One &delegateParams, int32& out)
{
	bool ret = false;
	if (IsValid(delegateParams.dataInfoPtr) && delegateParams.dataPtr)
	{
		if (delegateParams.dataInfoPtr->IsA<UNumericProperty>())
		{
			UNumericProperty *tProp = Cast<UNumericProperty>(delegateParams.dataInfoPtr);
			if (tProp)
			{
				if (tProp->IsInteger())
				{
					int64 val = tProp->GetSignedIntPropertyValue(delegateParams.dataPtr);
					out = (int32)val;
					ret = true;
				}
				else
				{
					WVLogW(TEXT("delegateParams not a int"))
				}
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
			}
		}
		else
		{
			WVLogW(TEXT("delegateParams not a number"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToFloat(const FWVEventDelegateParams_One& delegateParams, float& out)
{
	bool ret = false;
	if (IsValid(delegateParams.dataInfoPtr) && delegateParams.dataPtr)
	{
		if (delegateParams.dataInfoPtr->IsA<UNumericProperty>())
		{
			UNumericProperty *tProp = Cast<UNumericProperty>(delegateParams.dataInfoPtr);
			if (tProp)
			{
				if (tProp->IsFloatingPoint())
				{
					float val = tProp->GetFloatingPointPropertyValue(delegateParams.dataPtr);
					out = val;
					ret = true;
				}
				else
				{
					WVLogW(TEXT("delegateParams not a float"))
				}
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
			}
		}
		else
		{
			WVLogW(TEXT("delegateParams not a number"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToString(const FWVEventDelegateParams_One& delegateParams, FString& out)
{
	bool ret = false;
	if (IsValid(delegateParams.dataInfoPtr) && delegateParams.dataPtr)
	{
		if (delegateParams.dataInfoPtr->IsA<UStrProperty>())
		{
			UStrProperty *tProp = Cast<UStrProperty>(delegateParams.dataInfoPtr);
			if (tProp)
			{
				FString val = tProp->GetPropertyValue(delegateParams.dataPtr);
				out = val;
				ret = true;
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
			}
		}
		else
		{
			WVLogW(TEXT("delegateParams not a string"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToObj(const FWVEventDelegateParams_One& delegateParams, UObject*& out)
{
	bool ret = false;
	if (IsValid(delegateParams.dataInfoPtr) && delegateParams.dataPtr)
	{
		if (delegateParams.dataInfoPtr->IsA<UObjectProperty>())
		{
			UObjectProperty *tProp = Cast<UObjectProperty>(delegateParams.dataInfoPtr);
			if (tProp)
			{
				UObject *val = tProp->GetPropertyValue(delegateParams.dataPtr);
				out = val;
				ret = true;
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
			}
		}
		else
		{
			WVLogW(TEXT("delegateParams not a object"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}

bool UWVBlueprintFunctionLibrary::ConvEventOneParamsToStruct(const FWVEventDelegateParams_One& delegateParams, UProperty*& out)
{
	//this never gets called due to custom thunk
	return false;
}

bool UWVBlueprintFunctionLibrary::_ConvEventOneParamsToStruct(FWVEventDelegateParams_One& inParams,
	FWVEventDelegateParams_One& outParams)
{
	bool ret = false;
	if (IsValid(inParams.dataInfoPtr) && inParams.dataPtr)
	{
		if (!inParams.dataInfoPtr->IsA<UStructProperty>())
		{
			WVLogW(TEXT("delegateParams not a struct"))
			return ret;
		}
		if (!outParams.dataInfoPtr->IsA<UStructProperty>())
		{
			WVLogW(TEXT("out param not a struct"))
			return ret;
		}
		UStructProperty *inProp = Cast<UStructProperty>(inParams.dataInfoPtr);
		UStructProperty *outProp = Cast<UStructProperty>(outParams.dataInfoPtr);
		if (inProp && outProp)
		{
			if (inProp->Struct == outProp->Struct)
			{
				outProp->CopyCompleteValue(outParams.dataPtr, inParams.dataPtr);
				ret = true;
			}
			else
			{
				WVLogW(TEXT("inProp->Struct != outProp->Struct"))
			}
		}
		else
		{
			WVLogW(TEXT("some thing wrong"))
		}
	}
	else
	{
		WVLogW(TEXT("delegateParams invalid"))
	}
	return ret;
}
