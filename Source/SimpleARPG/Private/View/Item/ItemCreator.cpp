// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Item/ItemCreator.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"

void UItemCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(TEXT("ItemCreator_Test"), this, TEXT("Rec_Test"));
}

void UItemCreator::Rec_Test()
{
	WVLogI(TEXT("UItemCreator::Rec_Test"))
}
