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

void UWVBlueprintFunctionLibrary::ConvEventOneParamsToInt32(const FWVEventDelegateParams_One &delegateParams, int32& out)
{
	if (IsValid(delegateParams.dataInfoPtr))
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
				}
				else
				{
					WVLogW(TEXT("delegateParams not a int"))
				}
			}
			else
			{
				WVLogW(TEXT("some thing wrong"))
				WVLogW(*WVLog_Location)
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
}
