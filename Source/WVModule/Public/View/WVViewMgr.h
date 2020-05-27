// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
	UWVViewBase* _mainView;

public:
	static UWVViewMgr* GetInstance();
	void Cleanup();

	void SwitchMainView(UWVViewBase* view);

protected:
	UWVViewMgr();
	~UWVViewMgr();
};
