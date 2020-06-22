// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSys/ComboMachineComp.h"
#include "Gameplay/ActionCharacter.h"
#include "Engine/World.h"
#include "ComboSys/ComboNode.h"
#include "InputBufferSys/InputBufferController.h"
#include "InputBufferSys/InputBufferComp.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"

// Sets default values for this component's properties
UComboMachineComp::UComboMachineComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	_EntryNode = nullptr;
	_CurNode = nullptr;
}


// Called when the game starts
void UComboMachineComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!Class_ComboNode)
	{
		return;
	}

	AActionCharacter *tCharacter = Cast<AActionCharacter>(GetOwner());

	if (!tCharacter)
	{
		return;
	}
	
	if (!_EntryNode)
	{
		auto cfg_combo = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboRow>(EWVConfigName::Combo, tCharacter->GetActionCharacterName());
		if (cfg_combo)
		{
			if (cfg_combo->ComboInfoMap.Num() > 0)
			{
				_EntryNode = NewObject<UComboNode>(this, Class_ComboNode);
				_CurNode = _EntryNode;

				for (auto elem : cfg_combo->ComboInfoMap)
				{
					FString &comboName = elem.Key;
					FWVConfig_ComboInfo &comboInfo = elem.Value;
					
					for (int i = 0; i < comboInfo.ComboKeys.Num(); ++i)
					{
						int32 comboIndex = i;
						UComboNode *tNode = _CurNode->GetChild(comboInfo.ComboKeys[i].MatchKeys);

						if (!tNode)
						{
							tNode = NewObject<UComboNode>(this, Class_ComboNode);
							tNode->InitData(
								comboInfo.ComboKeys[i].MatchKeys,
								comboName,
								comboIndex,
								comboInfo.ComboPrioritys[i]
							);
							_CurNode->AddChild(tNode);
						}
						_CurNode = tNode;
					}

					_CurNode = _EntryNode;
				}
			}
		}
	}
}


// Called every frame
void UComboMachineComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UComboMachineComp::Start()
{
	if (IsRunning())
	{
		return false;
	}

	auto ret = _Step();

	if (ret && Callback_Start.IsBound())
	{
		Callback_Start.Broadcast();
	}
	
	return ret;
}

bool UComboMachineComp::Step()
{
	if (!IsRunning())
	{
		return false;
	}

	return _Step();
}

void UComboMachineComp::Resume()
{
	_CurNode = _EntryNode;

	if (Callback_Resume.IsBound())
	{
		Callback_Resume.Broadcast();
	}
}

bool UComboMachineComp::_Step()
{
	bool ret = false;
	
	AActionCharacter *tCharacter = Cast<AActionCharacter>(GetOwner());
	if (!tCharacter)
	{
		return ret;
	}

	auto cfg_combo = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboRow>(EWVConfigName::Combo, tCharacter->GetActionCharacterName());
	if (!cfg_combo)
	{
		return ret;
	}

	auto inputCtrl = Cast<AInputBufferController>(tCharacter->GetController());
	if (!inputCtrl)
	{
		return ret;
	}

	auto comp_inputBuffer = inputCtrl->GetInputBufferComp();
	if (!comp_inputBuffer)
	{
		return ret;
	}

	auto tNodes = _CurNode->GetChildren();
	for (auto tNode : tNodes)
	{
		const FString &comboName = tNode->GetComboName();
		const int32 &comboIndex = tNode->GetComboIndex();
		auto comboInfo = cfg_combo->ComboInfoMap.Find(comboName);

		if (comboInfo && comboIndex >= 0 && comboIndex < comboInfo->ComboKeys.Num())
		{
			auto &matchKeys = comboInfo->ComboKeys[comboIndex].MatchKeys;
			auto &matchStyles = comboInfo->ComboMatchStyles[comboIndex].MatchStyles;
			auto &matchDatas = comboInfo->ComboMatchDatas[comboIndex].MatchDatas;

			bool bMatch = comp_inputBuffer->MatchAll(matchKeys, matchStyles, matchDatas);

			if (bMatch && tNode->Condition())
			{
				tNode->Do();

				_CurNode = tNode;

				ret = true;

				break;
			}
		}
	}

	return ret;
}

UComboNode* UComboMachineComp::GetCurNode()
{
	if (IsRunning())
	{
		return _CurNode;
	}
	return nullptr;
}
