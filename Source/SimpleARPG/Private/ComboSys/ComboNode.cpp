// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSys/ComboNode.h"
#include "ComboSys/ComboMachineComp.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"
#include "Gameplay/ActionCharacter.h"

UComboNode::UComboNode()
{
	_ComboName = TEXT("");
	_ComboIndex = -1;
	_Priority = 0;
}

void UComboNode::InitData(const TArray<FKey> &InMatchKeys, const FString &InComboName, const int32 &InComboIndex, const int32 &InPriority)
{
	_ComboName = InComboName;
	_ComboIndex = InComboIndex;
	_Priority = InPriority;
	for (auto key : InMatchKeys)
	{
		_MatchKeysMap.Emplace(key, true);
	}
}

UComboNode* UComboNode::GetChild(const TArray<FKey> &InMatchKeys)
{
	for (auto tNode : _Children)
	{
		bool bFind = true;
		for (auto key : InMatchKeys)
		{
			if (!tNode->_MatchKeysMap.Find(key))
			{
				bFind = false;
				break;
			}
		}
		if (bFind)
		{
			return tNode;
		}
	}
	return nullptr;
}

void UComboNode::AddChild(UComboNode *InNode)
{
	int32 tIndex = -1;
	for (int32 i = 0; i < _Children.Num(); ++i)
	{
		if (InNode->GetPriority() > _Children[i]->GetPriority())
		{
			tIndex = i;
			break;
		}
	}
	if (tIndex < 0)
	{
		_Children.Push(InNode);
	}
	else
	{
		_Children.Insert(InNode, tIndex);
	}
}

AActor* UComboNode::GetOwner()
{
	auto outer = GetOuter();
	if (outer)
	{
		auto comboMachine = Cast<UComboMachineComp>(outer);
		if (comboMachine)
		{
			return comboMachine->GetOwner();
		}
	}
	return nullptr;
}

FString UComboNode::GetComboActionName()
{
	auto actionCharacter = Cast<AActionCharacter>(GetOwner());
	if (actionCharacter)
	{
		auto cfg = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboRow>(EWVConfigName::Combo, actionCharacter->GetActionCharacterName());
		if (cfg)
		{
			auto tComboInfo = cfg->ComboInfoMap.Find(_ComboName);
			if (tComboInfo)
			{
				if (_ComboIndex >= 0 && _ComboIndex < tComboInfo->ComboActionNames.Num())
				{
					return tComboInfo->ComboActionNames[_ComboIndex];
				}
			}
		}
	}
	return FString();
}

bool UComboNode::Condition_Implementation()
{
	auto tActionCharacter = Cast<AActionCharacter>(GetOwner());

	if (tActionCharacter && tActionCharacter->IsDodging())
	{
		return false;
	}
	
	return true;
}

void UComboNode::Do_Implementation()
{
	// FString str = FString::Printf(TEXT("%s_%d_"), *_ComboName, _ComboIndex);
	// for (auto elem : _MatchKeysMap)
	// {
	// 	if (elem.Value)
	// 	{
	// 		str += elem.Key.GetFName().ToString();
	// 	}
	// }
	// WVLogI(TEXT("%s"), *str)
}
