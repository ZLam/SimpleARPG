// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVViewMgr.generated.h"

UENUM()
enum class EWVViewZOrder : uint8
{
	MainView,
	Dialog,
	Tips,
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

public:
	static UWVViewMgr* GetInstance();
	void Cleanup();

protected:
	UWVViewMgr();
	~UWVViewMgr();
};
