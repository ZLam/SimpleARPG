// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Item/ItemCreator.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "View/Item/ItemView.h"

void UItemCreator::OnEnter()
{
	Super::OnEnter();
	
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateItemView, this, TEXT("Rec_OnCreateItemView"));
}

void UItemCreator::Rec_OnCreateItemView()
{
	UWVViewMgr::GetInstance()->PushFuncView<UItemView>(TEXT("WidgetBlueprint'/Game/View/Item/WidgetBP_ItemView.WidgetBP_ItemView_C'"));
}
