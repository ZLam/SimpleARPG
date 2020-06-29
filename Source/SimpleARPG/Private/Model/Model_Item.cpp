// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Model_Item.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"

UModel_Item::UModel_Item()
{
	
}

void UModel_Item::OnEnter()
{
	Super::OnEnter();
}

void UModel_Item::AddItem(int32 ItemId, int32 Num)
{
	if (Num > 0)
	{
		auto pNum = _ItemMap.Find(ItemId);
		if (!pNum)
		{
			_ItemMap.Emplace(ItemId);
		}
		_ItemMap[ItemId] += Num;
	}
}

int32 UModel_Item::GetItemNum(int32 ItemId)
{
	auto pNum = _ItemMap.Find(ItemId);
	return pNum ? *pNum : 0;
}

TArray<int32> UModel_Item::GetItemsForShow()
{
	TArray<int32> ret;

	if (_ItemMap.Num() <= 0)
	{
		return ret;
	}

	for (auto elem : _ItemMap)
	{
		ret.Add(elem.Key);
	}

	ret.StableSort(
		[](const int32 &a, const int32 &b) -> bool
		{
			auto cfg_a = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ItemRow>(EWVConfigName::Item, FName(*(FString::FromInt(a))));
			auto cfg_b = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ItemRow>(EWVConfigName::Item, FName(*(FString::FromInt(b))));

			if (!cfg_a || !cfg_b)
			{
				return false;
			}

			return cfg_a->quality > cfg_b->quality;
		}
	);

	return ret;
}
