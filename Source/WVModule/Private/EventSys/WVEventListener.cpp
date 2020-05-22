// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventListener.h"

FWVEventListener::FWVEventListener(UObject* inCaller, const FString& inFuncName):
caller(inCaller),
funcName(inFuncName),
func(nullptr),
delegateOne(),
validType(EWVEventListenerValid::None)
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::FWVEventListener==="))
}

FWVEventListener::FWVEventListener(UObject* inCaller, FWVEventDelegate_One inDelegateOne):
caller(inCaller),
delegateOne(inDelegateOne),
funcName(),
func(nullptr),
validType(EWVEventListenerValid::None)
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::FWVEventListener==="))
}

FWVEventListener::~FWVEventListener()
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::~FWVEventListener==="))
}

EWVEventListenerValid FWVEventListener::IsReady()
{
	if (!IsValid(caller))
	{
		validType = EWVEventListenerValid::Fatal;
		return validType;
	}
	
	if (validType == EWVEventListenerValid::None)
	{
		if (delegateOne.IsBound())
		{
			validType = EWVEventListenerValid::Delegate;
		}
		else if (!funcName.IsEmpty())
		{
			func = caller->FindFunction(FName(*funcName));
			if (func)
			{
				validType = EWVEventListenerValid::FuncName;
			}
		}
	}
	
	return  validType;
}
