// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Logger/WVLog.h"
#include "WVViewCreatorMgr.generated.h"

class UWVViewCreatorBase;

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewCreatorMgr : public UObject
{
	GENERATED_BODY()

protected:
	static UWVViewCreatorMgr *_instance;

	UPROPERTY()
	TMap<FString, UWVViewCreatorBase*> _creators;

	UPROPERTY()
	UGameInstance *_gameIns;

public:
	static UWVViewCreatorMgr* GetInstance();
	void Cleanup();

	void SetupGameInstance(UGameInstance *ins)
	{
		_gameIns = ins;
	}

	template<typename T>
	void RegistCreator(const FString &name);

protected:
	UWVViewCreatorMgr();
	~UWVViewCreatorMgr();
};

template <typename T>
void UWVViewCreatorMgr::RegistCreator(const FString &name)
{
	auto pCreator = _creators.Find(name);
	if (pCreator)
	{
		WVLogW(TEXT("creator %s has existed"), *name)
		return;
	}
	T *creator = NewObject<T>();
	creator->AddToRoot();
	creator->SetupGameInstance(_gameIns);
	creator->OnEnter();
	
	_creators.Add(name, creator);
	// _creators.Emplace(name, creator);
}
