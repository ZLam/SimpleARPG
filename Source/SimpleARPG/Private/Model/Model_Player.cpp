// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Model_Player.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"

UModel_Player::UModel_Player()
{
	_MoneyNum = 0;
}

void UModel_Player::OnEnter()
{
	Super::OnEnter();
}

int32 UModel_Player::GetMoney()
{
	return _MoneyNum;
}

void UModel_Player::SetMoney(int32 num, bool bFireEvent)
{
	if (num < 0)
	{
		num = 0;
	}
	_MoneyNum = num;
	if (bFireEvent)
	{
		UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::MoneyChange);
	}
}
