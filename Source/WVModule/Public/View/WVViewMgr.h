// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVGameInstance.h"
#include "Logger/WVLog.h"
#include "WVViewMgr.generated.h"

class UWVViewBase;

UENUM()
enum class EWVViewZOrder : uint8
{
	MainView	= 10,//主界面
	FuncView	= 20,//功能界面
	Dialog		= 30,//弹窗
	Tips		= 40,//提示框
};

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewMgr : public UObject
{
	GENERATED_BODY()

protected:
	static UWVViewMgr *_instance;

	UPROPERTY()
	UWVGameInstance *_gameIns;

	UPROPERTY()
	UWVViewBase* _mainView;

	UPROPERTY()
	TArray<UWVViewBase*> _funcViewArr;

	UPROPERTY()
	TArray<UWVViewBase*> _dialogArr;
	
	template<typename T>
	T* _AddView(EWVViewZOrder whatZOrder, const TCHAR *widgetBP_Path);

public:
	static UWVViewMgr* GetInstance();
	void Cleanup();

	void SetupGameInstance(UWVGameInstance *ins)
	{
		_gameIns = ins;
	}

	void SwitchMainView(UWVViewBase* view);

	template<typename T>
	T* SwitchMainView(const TCHAR *widgetBP_Path);

	void PushFuncView(UWVViewBase *view);

	template<typename T>
	T* PushFuncView(const TCHAR *widgetBP_Path);

	void PopFuncView(UWVViewBase *view = nullptr);

	void PushDialog(UWVViewBase *view);

	template<typename T>
	T* PushDialog(const TCHAR *widgetBP_Path);

	void PopDialog(UWVViewBase *view = nullptr);

protected:
	UWVViewMgr();
	~UWVViewMgr();
};

template <typename T>
T* UWVViewMgr::_AddView(EWVViewZOrder whatZOrder, const TCHAR* widgetBP_Path)
{
	T *ret = nullptr;
	auto cls = LoadClass<T>(nullptr, widgetBP_Path);

	if (!cls)
	{
		WVLogW(TEXT("load class'%s' fail"), widgetBP_Path)
		return ret;
	}

	auto w = _gameIns->GetWorld();
	if (w)
	{
		auto view = CreateWidget<T>(w, cls);
		if (view)
		{
			switch (whatZOrder)
			{
				case EWVViewZOrder::MainView:
				{
					SwitchMainView(view);
					break;
				}
				case EWVViewZOrder::FuncView:
				{
					PushFuncView(view);
					break;
				}
				case EWVViewZOrder::Dialog:
				{
					PushDialog(view);
					break;;
				}
			}

			ret = view;
		}
		else
		{
			WVLogW(TEXT("create view'%s' fail"), widgetBP_Path)
			return ret;
		}
	}

	return ret;
}

template <typename T>
T* UWVViewMgr::SwitchMainView(const TCHAR *widgetBP_Path)
{
	return _AddView<T>(EWVViewZOrder::MainView, widgetBP_Path);
}

template <typename T>
T* UWVViewMgr::PushFuncView(const TCHAR* widgetBP_Path)
{
	return _AddView<T>(EWVViewZOrder::FuncView, widgetBP_Path);
}

template <typename T>
T* UWVViewMgr::PushDialog(const TCHAR* widgetBP_Path)
{
	return _AddView<T>(EWVViewZOrder::Dialog, widgetBP_Path);
}
