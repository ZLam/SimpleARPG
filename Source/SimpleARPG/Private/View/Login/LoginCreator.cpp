// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Login/LoginCreator.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "View/Login/LoginView.h"

void ULoginCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateLoginView, this, TEXT("Rec_OnCreateLoginView"));
}

void ULoginCreator::Rec_OnCreateLoginView()
{
	UWVViewMgr::GetInstance()->SwitchMainView<ULoginView>(TEXT("WidgetBlueprint'/Game/View/Login/WidgetBP_LoginView.WidgetBP_LoginView_C'"));
}
