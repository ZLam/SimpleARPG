// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSys/ComboNode.h"
#include "ComboSys/ComboMachineComp.h"

UComboNode::UComboNode()
{
	_ComboName = TEXT("");
	_ComboIndex = -1;
}

void UComboNode::InitData(const TArray<FKey> &InMatchKeys, const FString &InComboName, const int32 &InComboIndex)
{
	_ComboName = InComboName;
	_ComboIndex = InComboIndex;
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
	_Children.Push(InNode);
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

bool UComboNode::Condition_Implementation()
{
	return true;
}

void UComboNode::Do_Implementation()
{
	
}
