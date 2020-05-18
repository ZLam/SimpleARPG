// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WVBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EWVLogVerbosity : uint8
{
	Info		UMETA(DisplayName = "Info"),
	Warning		UMETA(DisplayName = "Warning"),
	Fatal		UMETA(DisplayName = "Fatal"),
	Error		UMETA(DisplayName = "Error"),
};

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
};
