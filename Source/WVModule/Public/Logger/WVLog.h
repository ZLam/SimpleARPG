// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVLog.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WVLogger, All, All);

#define WVLog_Location (FString::Printf(TEXT("%s : %d"), *FString(__FUNCTION__), __LINE__))

#define WVLogScreenDuration 10

#define WVLogI(fmt, ...) FWVLog::GetInstance()->Info(FString::Printf(fmt, __VA_ARGS__), true, true, WVLogScreenDuration);
#define WVLogW(fmt, ...) FWVLog::GetInstance()->Warning(FString::Printf(fmt, __VA_ARGS__), true, true, WVLogScreenDuration);
#define WVLogF(fmt, ...) FWVLog::GetInstance()->Fatal(FString::Printf(fmt, __VA_ARGS__), true, true, WVLogScreenDuration);
#define WVLogE(fmt, ...) FWVLog::GetInstance()->Error(FString::Printf(fmt, __VA_ARGS__), true, true, WVLogScreenDuration);

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

protected:
	FWVLog();
	void Init();
	
public:
	~FWVLog();
};
