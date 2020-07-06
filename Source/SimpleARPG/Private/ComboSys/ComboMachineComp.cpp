// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSys/ComboMachineComp.h"
#include "Gameplay/ActionCharacter.h"
#include "Engine/World.h"
#include "ComboSys/ComboNode.h"
#include "InputBufferSys/InputBufferController.h"
#include "InputBufferSys/InputBufferComp.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/Logger/WVLog.h"

// Sets default values for this component's properties
UComboMachineComp::UComboMachineComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	_EntryNode = nullptr;
	_CurNode = nullptr;
	_CurComboActionIndex = 0;
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

	AActionCharacter *tCharacter = Cast<AActionCharacter>(GetOwner());

	if (tCharacter->IsAI)
	{
		SetupCurCombo_AI();
	}

	auto ret = _Step();

	if (ret)
	{
		if (tCharacter)
		{
			if (tCharacter->_State == EWVActionCharacterState::ReadyAtk)
			{
				tCharacter->_State = EWVActionCharacterState::Atking;
			}
		}
	}
	else
	{
		Resume();
	}
	
	return ret;
}

bool UComboMachineComp::Step()
{
	if (!IsRunning())
	{
		return false;
	}

	auto ret = _Step();

	// 开了的话，可以中途打新的combo，在后摇时。。。：）
	// if (!ret)
	// {
	// 	Resume();
	// }

	return ret;
}

void UComboMachineComp::Resume()
{
	_CurNode = _EntryNode;

	AActionCharacter *tCharacter = Cast<AActionCharacter>(GetOwner());
	
	if (tCharacter)
	{
		if (tCharacter->_State == EWVActionCharacterState::Atking)
		{
			tCharacter->_State = EWVActionCharacterState::ReadyAtk;
		}

		if (tCharacter->IsAI)
		{
			ClearCurCombo_AI();
		}
		
		

		// @TEMP 暂时拿来看看按键
		UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::ComboResumeExecute);
		//
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

	AInputBufferController *inputCtrl = nullptr;
	UInputBufferComp *comp_inputBuffer = nullptr;
	if (!tCharacter->IsAI)
	{
		inputCtrl = Cast<AInputBufferController>(tCharacter->GetController());
		if (!inputCtrl)
		{
			return ret;
		}

		comp_inputBuffer = inputCtrl->GetInputBufferComp();
		if (!comp_inputBuffer)
		{
			return ret;
		}
	}

	auto cfg_combo = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboRow>(EWVConfigName::Combo, tCharacter->GetActionCharacterName());
	if (!cfg_combo)
	{
		return ret;
	}

	auto tNodes = _CurNode->GetChildren();
	for (auto tNode : tNodes)
	{
		const FString &comboName = tNode->GetComboName();
		const int32 &comboIndex = tNode->GetComboIndex();

		if (!tCharacter->IsAI)
		{
			//玩家自己的处理
			
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

					// @TEMP 暂时拿来看看按键
					FWVParams_ComboStepExecute tParams;
					tParams.ComboMatchKeys = matchKeys;
					UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::ComboStepExecute, &tParams);
					//

					break;
				}
			}
		}
		else
		{
			//AI
			
			if (!_CurComboName_AI.IsEmpty())
			{
				auto comboInfo = cfg_combo->ComboInfoMap.Find(_CurComboName_AI);
				if (comboInfo && _CurComboActionIndex >= 0 && _CurComboActionIndex < comboInfo->ComboKeys.Num())
				{
					bool bMatch = tNode->IsMatchKeys(comboInfo->ComboKeys[_CurComboActionIndex].MatchKeys);
					if (bMatch && tNode->Condition())
					{
						WVLogI(TEXT("%s"), *(tNode->GetComboActionName()))
						
						tNode->Do();
						_CurNode = tNode;
						ret = true;
						_CurComboActionIndex++;
						
						break;;
					}
				}
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

void UComboMachineComp::SetupCurCombo_AI_Implementation()
{
	if (!_CurComboName_AI.IsEmpty())
	{
		return;
	}
	
	AActionCharacter *tCharacter = Cast<AActionCharacter>(GetOwner());
	if (!tCharacter)
	{
		return;
	}

	auto cfg_combo = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboRow>(EWVConfigName::Combo, tCharacter->GetActionCharacterName());
	if (!cfg_combo)
	{
		return;
	}

	TArray<FString> keys;
	
	cfg_combo->ComboInfoMap.GetKeys(keys);
	if (keys.Num() > 0)
	{
		auto index = FMath::RandRange(0, keys.Num() - 1);
		_CurComboName_AI = FString(keys[index]);
	}

	WVLogI(TEXT("%s combo : %s"), *(tCharacter->GetName()), *_CurComboName_AI)
}

void UComboMachineComp::ClearCurCombo_AI()
{
	_CurComboName_AI.Empty();
	_CurComboActionIndex = 0;
}
