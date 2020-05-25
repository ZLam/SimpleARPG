// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/WVModelMgr.h"
#include "WVModule.h"

UWVModelMgr *UWVModelMgr::_instance = nullptr;

UWVModelMgr::UWVModelMgr():
_models()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVModelMgr::UWVModelMgr==="))
}

UWVModelMgr::~UWVModelMgr()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVModelMgr::~UWVModelMgr==="))
}

UWVModelMgr* UWVModelMgr::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVModelMgr>();
		_instance->AddToRoot();
	}
	return _instance;
}

void UWVModelMgr::Cleanup()
{
	auto ite = _models.CreateIterator();
	while (ite)
	{
		ite.Value()->OnExit();
		ite.Value()->RemoveFromRoot();
		++ite;
	}
	_models.Empty();
	_instance->RemoveFromRoot();
}

FString UWVModelMgr::GetModelName(EWVModelName enumName)
{
	FText txt = UEnum::GetDisplayValueAsText<EWVModelName>(enumName);
	return txt.ToString();
}

UWVModelBase* UWVModelMgr::GetModel(EWVModelName enumName)
{
	auto pModel = _models.Find(enumName);
	if (!pModel)
	{
		return nullptr;
	}
	return *pModel;
}
