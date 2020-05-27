// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestHUD.h"
#include "Blueprint/UserWidget.h"
#include "Test/TestUserWidget.h"
#include "WVModule/Public/Logger/WVLog.h"

void ATestHUD::BeginPlay()
{	
	// if (IsValid(_mainViewClass))
	// {
	// 	auto cls = LoadClass<UTestUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Test/BP_TestMainView.BP_TestMainView_C'"));
	//
	// 	if (cls)
	// 	{
	// 		_mainView = CreateWidget<UTestUserWidget>(GetWorld(), cls);
	// 		if (_mainView)
	// 		{
	// 			_mainView->AddToViewport();
	// 		}
	// 	}
	// 	
	// 	// _mainView = CreateWidget<UTestUserWidget>(GetWorld(), _mainViewClass);
	// 	// if (_mainView)
	// 	// {
	// 	// 	_mainView->AddToViewport();
	// 	// }
	// }
}
