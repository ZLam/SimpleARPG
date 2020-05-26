// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Model_Player.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"

void UModel_Player::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(TEXT("ModelPlayer_Test"), this, TEXT("Rec_Test"));
}

void UModel_Player::Rec_Test()
{
	WVLogI(TEXT("UModel_Player::Rec_Test"))
}
