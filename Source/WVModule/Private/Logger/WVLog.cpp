// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger/WVLog.h"
#include "WVModule.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"

DEFINE_LOG_CATEGORY(WVLogger);

TSharedPtr<FWVLog> FWVLog::_instance = nullptr;

FWVLog::FWVLog():
_logScreenColors()
{
	UE_LOG(LogWVModule, Display, TEXT("===FWVLog::FWVLog==="));
}

FWVLog::~FWVLog()
{
	UE_LOG(LogWVModule, Display, TEXT("===FWVLog::~FWVLog==="));
}

void FWVLog::Init()
{
	_logScreenColors.Emplace(EWVLogVerbosity::Info, FColor(0, 255, 0));
	_logScreenColors.Emplace(EWVLogVerbosity::Warning, FColor(255, 255, 0));
	_logScreenColors.Emplace(EWVLogVerbosity::Fatal, FColor(255, 0, 0));
	_logScreenColors.Emplace(EWVLogVerbosity::Error, FColor(255, 0, 0));
}

TSharedPtr<FWVLog> FWVLog::GetInstance()
{
	if (!_instance.IsValid())
	{
		_instance = MakeShareable(new FWVLog());
		_instance->Init();
	}
	return _instance;
}

void FWVLog::Cleanup()
{
	if (_instance.IsValid())
	{
		_instance.Reset();
	}
}

void FWVLog::Info(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	Log(strLog, EWVLogVerbosity::Info, bPrintToLog, bPrintToScreen, duration);
}

void FWVLog::Warning(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	Log(strLog, EWVLogVerbosity::Warning, bPrintToLog, bPrintToScreen, duration);
}

void FWVLog::Fatal(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	Log(strLog, EWVLogVerbosity::Fatal, bPrintToLog, bPrintToScreen, duration);
}

void FWVLog::Error(const FString& strLog, bool bPrintToLog, bool bPrintToScreen, float duration)
{
	Log(strLog, EWVLogVerbosity::Error, bPrintToLog, bPrintToScreen, duration);
}

void FWVLog::Log(const FString& strLog, EWVLogVerbosity logVerbosity, bool bPrintToLog, bool bPrintToScreen,
	float duration)
{
#if !NO_LOGGING
	FColor color_logScreen;
	auto tColor = _logScreenColors.Find(logVerbosity);
	if (tColor)
	{
		color_logScreen = *tColor;
	}

	if (bPrintToScreen)
	{
		auto strLogVerbosity = UEnum::GetDisplayValueAsText(logVerbosity);
		duration = duration <= 0 ? 600 : duration;
		auto strOut = FString::Printf(TEXT("[%s]%s"), *strLogVerbosity.ToString(), *strLog);
		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, duration, color_logScreen, strOut);
	}

	if (bPrintToLog)
	{
		auto now = FDateTime::Now();
		auto strOut = FString::Printf(TEXT("[%d:%d:%d:%d]%s"),
			now.GetHour(), now.GetMinute(), now.GetSecond(), now.GetMillisecond(),
			*strLog
		);

		switch (logVerbosity)
		{
			case EWVLogVerbosity::Info:
			{
				UE_LOG(WVLogger, Display, TEXT("%s"), *strOut);
				break;
			}
			case EWVLogVerbosity::Warning:
			{
				UE_LOG(WVLogger, Warning, TEXT("%s"), *strOut);
				break;
			}
			case  EWVLogVerbosity::Fatal:
			{
				UE_LOG(WVLogger, Fatal, TEXT("%s"), *strOut);
				break;
			}
			case EWVLogVerbosity::Error:
			{
				UE_LOG(WVLogger, Error, TEXT("%s"), *strOut);
				break;
			}
		}
	}
#endif
}
