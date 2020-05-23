// Fill out your copyright notice in the Description page of Project Settings.


#include "WVBlueprintFunctionLibrary.h"
#include "Logger/WVLog.h"
#include "EventSys/WVEventDispatcher.h"

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


