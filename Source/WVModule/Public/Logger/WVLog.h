// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVBlueprintFunctionLibrary.h"

DECLARE_LOG_CATEGORY_EXTERN(WVLogger, All, All);

#define WVLOG_INFO(strLog) FWVLog::GetInstance()->Info(strLog, true, true, 10);
#define WVLOG_WARNING(strLog) FWVLog::GetInstance()->Warning(strLog, true, true, 10);
#define WVLOG_FATAL(strLog) FWVLog::GetInstance()->Fatal(strLog, true, true, 10);
#define WVLOG_ERROR(strLog) FWVLog::GetInstance()->Error(strLog, true, true, 10);

/**
 * 
 */
class WVMODULE_API FWVLog
{
public:
	static TSharedPtr<FWVLog> GetInstance();
	static void Cleanup();

	void Info(const FString &strLog, bool bPrintToLog, bool bPrintToScreen, float duration);
	void Warning(const FString &strLog, bool bPrintToLog, bool bPrintToScreen, float duration);
	void Fatal(const FString &strLog, bool bPrintToLog, bool bPrintToScreen, float duration);
	void Error(const FString &strLog, bool bPrintToLog, bool bPrintToScreen, float duration);
	void Log(const FString &strLog, EWVLogVerbosity logVerbosity, bool bPrintToLog, bool bPrintToScreen, float duration);

private:
	TMap<EWVLogVerbosity, FColor> _logScreenColors;
	static TSharedPtr<FWVLog> _instance;

private:
	void init();

public:
	~FWVLog();

private:
	FWVLog();
};
