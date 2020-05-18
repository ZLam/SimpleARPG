// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVConfigUtil.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVConfigUtil : public UObject
{
	GENERATED_BODY()

public:
	~UWVConfigUtil();
	static UWVConfigUtil* GetInstance();
	static void Cleanup();

private:
	static UWVConfigUtil* _instance;

private:
	UWVConfigUtil();
};
