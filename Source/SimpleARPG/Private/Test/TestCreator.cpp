// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCreator.h"
#include "Test/TestUserWidget.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/WVGameInstance.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"

void UTestCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateTestView, this, TEXT("Rec_CreateTestView"));
}

void UTestCreator::Rec_CreateTestView()
{
	auto cls = LoadClass<UTestUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Test/BP_TestMainView.BP_TestMainView_C'"));
	if (cls)
	{
		auto w =	_gameIns->GetWorld();
		if (w)
		{
			auto view = CreateWidget<UTestUserWidget>(w, cls);
			if (view)
			{
				UWVViewMgr::GetInstance()->SwitchMainView(view);
			}
		}
	}
}
