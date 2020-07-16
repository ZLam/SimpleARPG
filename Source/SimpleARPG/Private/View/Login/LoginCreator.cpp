// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Login/LoginCreator.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "View/Login/LoginView.h"
#include "View/Login/ChooseLevelView.h"

void ULoginCreator::OnEnter()
{
	Super::OnEnter();
	
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateLoginView, this, TEXT("Rec_OnCreateLoginView"));
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateChooseLevelView, this, TEXT("Rec_OnCreateChooseLevelView"));
}

void ULoginCreator::Rec_OnCreateLoginView()
{
	UWVViewMgr::GetInstance()->SwitchMainView<ULoginView>(TEXT("WidgetBlueprint'/Game/View/Login/WidgetBP_LoginView.WidgetBP_LoginView_C'"));
}

void ULoginCreator::Rec_OnCreateChooseLevelView()
{
	UWVViewMgr::GetInstance()->PushFuncView<UChooseLevelView>(TEXT("WidgetBlueprint'/Game/View/Login/WidgetBP_ChooseLevelView.WidgetBP_ChooseLevelView_C'"));
}
