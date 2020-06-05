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

public:
	static UWVViewMgr* GetInstance();
	void Cleanup();

	void SetupGameInstance(UWVGameInstance *ins)
	{
		_gameIns = ins;
	}

	void SwitchMainView(UWVViewBase* view);

	template<typename T>
	void SwitchMainView(const TCHAR *widgetBP_Path);

protected:
	UWVViewMgr();
	~UWVViewMgr();
};

template <typename T>
void UWVViewMgr::SwitchMainView(const TCHAR *widgetBP_Path)
{
	auto cls = LoadClass<T>(nullptr, widgetBP_Path);
	
	if (!cls)
	{
		WVLogW(TEXT("load class'%s' fail"), widgetBP_Path)
	}

	auto w = _gameIns->GetWorld();
	if (w)
	{
		auto view = CreateWidget<T>(w, cls);
		if (view)
		{
			SwitchMainView(view);
		}
		else
		{
			WVLogW(TEXT("create view'%s' fail"), widgetBP_Path)
		}
	}
}
