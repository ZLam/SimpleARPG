// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVEventHandler.generated.h"

struct FWVEventParams_BP;
struct FWVEventListener;

UENUM()
enum class EWVEventHandlerRet : uint8
{
	Fail,
	Success,
};

/**
 *
 */
UCLASS()
class WVMODULE_API UWVEventHandler : public UObject
{
	GENERATED_BODY()

public:
	TDoubleLinkedList<TSharedPtr<FWVEventListener>> _listeners;
	TArray<TSharedPtr<FWVEventListener>> _toDeleteArr;
	
	UWVEventHandler():
	_listeners()
	{
		UE_LOG(LogTemp, Display, TEXT("===FWVEventHandler::FWVEventHandler==="))
	}
	~UWVEventHandler()
	{
		UE_LOG(LogTemp, Display, TEXT("===FWVEventHandler::~FWVEventHandler==="))
	}

	void Add(TSharedPtr<FWVEventListener> listener);
	void Delete(UObject* inObj);
	void Delete(TSharedPtr<FWVEventListener> listener);
	EWVEventHandlerRet FireEvent(void* params = nullptr);
	EWVEventHandlerRet FireEvent_BP(FWVEventParams_BP &params_bp);
};
