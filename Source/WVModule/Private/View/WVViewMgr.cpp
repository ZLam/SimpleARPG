// Fill out your copyright notice in the Description page of Project Settings.


#include "View/WVViewMgr.h"
#include "WVModule.h"
#include "View/WVViewBase.h"

UWVViewMgr *UWVViewMgr::_instance = nullptr;

UWVViewMgr::UWVViewMgr()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVViewMgr::UWVViewMgr==="))
}

UWVViewMgr::~UWVViewMgr()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVViewMgr::~UWVViewMgr==="))
}

UWVViewMgr* UWVViewMgr::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVViewMgr>();
		_instance->AddToRoot();
	}
	return _instance;
}

void UWVViewMgr::Cleanup()
{
	_instance->RemoveFromRoot();
}

void UWVViewMgr::SwitchMainView(UWVViewBase* view)
{
	if (!IsValid(view))
	{
		return;
	}
	if (IsValid(_mainView))
	{
		_mainView->RemoveFromParent();
	}
	_mainView = view;
	_mainView->AddToViewport((int32)EWVViewZOrder::MainView);
}
