// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Main/MainCreator.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "View/Main/MainView.h"

void UMainCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateMainView, this, TEXT("Rec_OnCreateMainView"));
}

void UMainCreator::Rec_OnCreateMainView()
{
	UWVViewMgr::GetInstance()->SwitchMainView<UMainView>(TEXT("WidgetBlueprint'/Game/View/Main/WidgetBP_MainView.WidgetBP_MainView_C'"));
}
