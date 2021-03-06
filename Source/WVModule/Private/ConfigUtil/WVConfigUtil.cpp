﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigUtil/WVConfigUtil.h"
#include "WVModule.h"

UWVConfigUtil* UWVConfigUtil::_instance = nullptr;

UWVConfigUtil::UWVConfigUtil():
_pathMap(),
_configMap()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVConfigUtil::UWVConfigUtil==="))
}

UWVConfigUtil::~UWVConfigUtil()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVConfigUtil::~UWVConfigUtil==="))

	_instance = nullptr;
}

UWVConfigUtil* UWVConfigUtil::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVConfigUtil>();
		_instance->AddToRoot();
		_instance->Init();
	}
	return _instance;
}

void UWVConfigUtil::Cleanup()
{
	if (IsValid(_instance))
	{
		_instance->RemoveFromRoot();
	}
}

void UWVConfigUtil::Init()
{
	/**
	 * 添加DataTable配置路径
	 */
	_pathMap.Emplace(EWVConfigName::Locomotion, TEXT("DataTable'/Game/ConfigData/DataTable_Locomotion.DataTable_Locomotion'"));
	_pathMap.Emplace(EWVConfigName::Attribute, TEXT("DataTable'/Game/ConfigData/DataTable_Attribute.DataTable_Attribute'"));
	_pathMap.Emplace(EWVConfigName::Combo, TEXT("DataTable'/Game/ConfigData/DataTable_Combo.DataTable_Combo'"));
	_pathMap.Emplace(EWVConfigName::ComboAction, TEXT("DataTable'/Game/ConfigData/DataTable_ComboAction.DataTable_ComboAction'"));
	_pathMap.Emplace(EWVConfigName::Item, TEXT("DataTable'/Game/ConfigData/DataTable_Item.DataTable_Item'"));
	_pathMap.Emplace(EWVConfigName::Shop, TEXT("DataTable'/Game/ConfigData/DataTable_Shop.DataTable_Shop'"));
	_pathMap.Emplace(EWVConfigName::ShopGood, TEXT("DataTable'/Game/ConfigData/DataTable_ShopGoods.DataTable_ShopGoods'"));
}

UDataTable* UWVConfigUtil::GetConfig(EWVConfigName configName)
{
	UDataTable *ret = _configMap.FindRef(configName);
	if (ret)
	{
		return ret;
	}

	const FString *path = _pathMap.Find(configName);

	if (path)
	{
		ret = LoadObject<UDataTable>(nullptr, **path);
		if (ret)
		{
			_configMap.Add(configName, ret);
			return ret;
		}
	}

	return nullptr;
}
