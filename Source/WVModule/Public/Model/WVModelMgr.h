// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Logger/WVLog.h"
#include "WVModelCenter.h"
#include "Model/WVModelBase.h"
#include "WVModelMgr.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVModelMgr : public UObject
{
	GENERATED_BODY()

protected:
	static UWVModelMgr *_instance;

	UPROPERTY()
	TMap<EWVModelName, UWVModelBase*> _models;

public:
	static UWVModelMgr* GetInstance();
	void Cleanup();

	FString GetModelName(EWVModelName enumName);
	
	template<typename T>
	void RegistModel(EWVModelName enumName);

	template<typename T>
	T* GetModel(EWVModelName enumName);

	UWVModelBase* GetModel(EWVModelName enumName);

protected:
	UWVModelMgr();
	~UWVModelMgr();
};

template <typename T>
void UWVModelMgr::RegistModel(EWVModelName enumName)
{
	auto pModel = _models.Find(enumName);
	if (pModel)
	{
		WVLogW(TEXT("%s has existed"), *GetModelName(enumName))
		return;
	}
	T *model = NewObject<T>();
	model->AddToRoot();
	model->OnEnter();
	_models.Add(enumName, model);
}

template <typename T>
T* UWVModelMgr::GetModel(EWVModelName enumName)
{
	UWVModelBase *pModel = GetModel(enumName);
	if (!pModel)
	{
		return nullptr;
	}
	return Cast<T>(pModel);
}
