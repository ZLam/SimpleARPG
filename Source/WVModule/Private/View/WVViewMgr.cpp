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

	_instance = nullptr;
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

void UWVViewMgr::PushFuncView(UWVViewBase* view)
{
	if (!IsValid(view))
	{
		return;
	}
	_funcViewArr.Push(view);
	view->AddToViewport((int32)EWVViewZOrder::FuncView);
}

void UWVViewMgr::PopFuncView(UWVViewBase* view)
{
	if (_funcViewArr.Num() > 0)
	{
		if (view)
		{
			for (int32 i = _funcViewArr.Num() - 1; i >= 0; --i)
			{
				if (_funcViewArr[i] == view)
				{
					_funcViewArr[i]->RemoveFromParent();
					_funcViewArr.RemoveAt(i);
					break;
				}
			}
		}
		else
		{
			_funcViewArr[_funcViewArr.Num() - 1]->RemoveFromParent();
		}
	}
}

void UWVViewMgr::PushDialog(UWVViewBase* view)
{
	if (!IsValid(view))
	{
		return;
	}
	_dialogArr.Push(view);
	view->AddToViewport((int32)EWVViewZOrder::Dialog);
}

void UWVViewMgr::PopDialog(UWVViewBase* view)
{
	if (_dialogArr.Num() > 0)
	{
		if (view)
		{
			for (int32 i = _dialogArr.Num() - 1; i >= 0; --i)
			{
				if (_dialogArr[i] == view)
				{
					_dialogArr[i]->RemoveFromParent();
					_dialogArr.RemoveAt(i);
					break;
				}
			}
		}
		else
		{
			_dialogArr[_dialogArr.Num() - 1]->RemoveFromParent();
		}
	}
}
