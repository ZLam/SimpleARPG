// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Shop/ShopCreator.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "View/Shop/ShopView.h"

void UShopCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateShopView, this, TEXT("Rec_OnCreateShopView"));
}

void UShopCreator::Rec_OnCreateShopView()
{
	UWVViewMgr::GetInstance()->PushFuncView<UShopView>(TEXT("WidgetBlueprint'/Game/View/Shop/WidgetBP_ShopView.WidgetBP_ShopView_C'"));	//记住加_C
}
