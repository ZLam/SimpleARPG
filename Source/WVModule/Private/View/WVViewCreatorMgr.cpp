// Fill out your copyright notice in the Description page of Project Settings.


#include "View/WVViewCreatorMgr.h"
#include "WVModule.h"
#include "View/WVViewCreatorBase.h"

UWVViewCreatorMgr *UWVViewCreatorMgr::_instance = nullptr;

UWVViewCreatorMgr::UWVViewCreatorMgr():
_creators()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVViewCreatorMgr::UWVViewCreatorMgr==="))
}

UWVViewCreatorMgr::~UWVViewCreatorMgr()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVViewCreatorMgr::~UWVViewCreatorMgr==="))

	_instance = nullptr;
}

UWVViewCreatorMgr* UWVViewCreatorMgr::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVViewCreatorMgr>();
		_instance->AddToRoot();
	}
	return _instance;
}

void UWVViewCreatorMgr::Cleanup()
{
	auto ite = _creators.CreateIterator();
	while (ite)
	{
		ite.Value()->OnExit();
		ite.Value()->RemoveFromRoot();
		++ite;
	}
	_creators.Empty();
	_instance->RemoveFromRoot();
}
