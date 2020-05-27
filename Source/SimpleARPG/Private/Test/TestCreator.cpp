// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCreator.h"
#include "Test/TestUserWidget.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "Engine/Engine.h"

void UTestCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateTestView, this, TEXT("Rec_CreateTestView"));
}

void UTestCreator::Rec_CreateTestView()
{
	WVLogI(TEXT("111"))
	auto cls = LoadClass<UTestUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Test/BP_TestMainView.BP_TestMainView_C'"));
	if (cls)
	{
		WVLogI(TEXT("222"))



		auto w =	_gameIns->GetWorld();
		if (w)
		{
			WVLogI(TEXT("333"))

			auto view = CreateWidget<UTestUserWidget>(w, cls);
			if (view)
			{
				WVLogI(TEXT("444"))
					UWVViewMgr::GetInstance()->SwitchMainView(view);
			}
		}

	}
}
