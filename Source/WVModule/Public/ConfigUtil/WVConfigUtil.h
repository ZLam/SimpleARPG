// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVConfigCenter.h"
#include "Engine/DataTable.h"
#include "WVConfigUtil.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVConfigUtil : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<EWVConfigName, FString> _pathMap;
	UPROPERTY()
	TMap<EWVConfigName, UDataTable*> _configMap;
	
	
	static UWVConfigUtil* _instance;

public:
	UDataTable* GetConfig(EWVConfigName configName);
	
	template<typename T>
	T* GetConfigRowData(EWVConfigName configName, const FName &rowName);

	template<typename T>
	void GetConfigAllRowData(EWVConfigName configName, TArray<T> &outArr);

	template<typename T>
	void ForeachRow(EWVConfigName configName, TFunctionRef<void(const FName& Key, const T& Value)> predicate);
	
	const TMap<EWVConfigName, FString>& GetPathMap() const
	{
		return _pathMap;
	}

private:
	void Init();

public:
	static UWVConfigUtil* GetInstance();
	void Cleanup();
	
protected:
	UWVConfigUtil();
	~UWVConfigUtil();
};

template <typename T>
T* UWVConfigUtil::GetConfigRowData(EWVConfigName configName, const FName& rowName)
{
	auto config = GetConfig(configName);
	if (!config)
	{
		return nullptr;
	}
	return config->FindRow<T>(rowName, nullptr);
}

template <typename T>
void UWVConfigUtil::GetConfigAllRowData(EWVConfigName configName, TArray<T>& outArr)
{
	auto config = GetConfig(configName);
	if (!config)
	{
		return;
	}
	config->GetAllRows(nullptr, outArr);
}

template <typename T>
void UWVConfigUtil::ForeachRow(EWVConfigName configName, TFunctionRef<void(const FName& Key, const T& Value)> predicate)
{
	auto config = GetConfig(configName);
	if (!config)
	{
		return;
	}
	config->ForeachRow(nullptr, predicate);
}


