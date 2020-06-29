// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameHelper.h"
#include "WVModule/Public/Model/WVModelMgr.h"
#include "Model/Model_Player.h"
#include "Model/Model_Item.h"

FWVGameResData UMyGameHelper::GetGameResNum(const FWVGameResData& InGameResData)
{
	FWVGameResData tGameResData = InGameResData;
	
	if (InGameResData.GameResType == EWVGameResType::Money)
	{
		auto modelPlayer = Cast<UModel_Player>(UWVModelMgr::GetInstance()->GetModel(EWVModelName::Player));
		if (modelPlayer)
		{
			tGameResData.Num = modelPlayer->GetMoney();
		}
	}
	else if (InGameResData.GameResType == EWVGameResType::Item)
	{
		auto modelItem = Cast<UModel_Item>(UWVModelMgr::GetInstance()->GetModel(EWVModelName::Item));
		if (modelItem)
		{
			tGameResData.Num = modelItem->GetItemNum(tGameResData.Id);
		}
	}

	return tGameResData;
}

FLinearColor UMyGameHelper::GetQualityColor(EWVQualityType InQuality)
{
	FLinearColor ret;
	switch (InQuality)
	{
		case EWVQualityType::White:
		{
			ret.R = 1.0f; ret.G = 1.0f; ret.B = 1.0f; ret.A = 0.5f;
			break;
		}
		case EWVQualityType::Green:
		{
			ret.R = 0.0f; ret.G = 1.0f; ret.B = 0.0f; ret.A = 0.5f;
			break;
		}
		case EWVQualityType::Blue:
		{
			ret.R = 0.0f; ret.G = 0.0f; ret.B = 1.0f; ret.A = 0.5f;
			break;
		}
		case EWVQualityType::Purple:
		{
			ret.R = 1.0f; ret.G = 0.0f; ret.B = 1.0f; ret.A = 0.5f;
			break;
		}
		case EWVQualityType::Orange:
		{
			ret.R = 1.0f; ret.G = 0.5f; ret.B = 0.0f; ret.A = 0.5f;
			break;
		}
	}
	return ret;
}
