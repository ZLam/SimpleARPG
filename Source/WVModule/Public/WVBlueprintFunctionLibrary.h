// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WVEventCenter.h"
#include "EventSys/WVEventDelegate.h"
#include "WVBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogI(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogW(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogF(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogE(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintPure)
	static FString ConvToEventSignature(EWVEventCategory inCategory, EWVEventName inEventName);

	UFUNCTION(BlueprintCallable)
	static void AddListener_FN(const FString &inEventSignature, UObject *inCaller, const FString &inFuncName);

	UFUNCTION(BlueprintCallable)
	static void AddListener_FN_SP(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, const FString &inFuncName);

	UFUNCTION(BlueprintCallable)
	static void AddListener_DL(const FString &inEventSignature, UObject *inCaller, FWVEventDelegate_One inDelegateOne);

	UFUNCTION(BlueprintCallable)
	static void AddListener_DL_SP(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, FWVEventDelegate_One inDelegateOne);

	UFUNCTION(BlueprintCallable)
	static void FireEvent(const FString &inEventSignature);

	UFUNCTION(BlueprintCallable)
	static void FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName);
};
