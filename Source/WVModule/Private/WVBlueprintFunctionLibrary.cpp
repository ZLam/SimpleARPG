// Fill out your copyright notice in the Description page of Project Settings.


#include "WVBlueprintFunctionLibrary.h"
#include "Logger/WVLog.h"

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
