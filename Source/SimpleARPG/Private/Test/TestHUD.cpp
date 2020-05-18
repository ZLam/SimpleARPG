// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestHUD.h"
#include "Blueprint/UserWidget.h"
#include "SimpleARPG/Public/Test/TestUserWidget.h"

void ATestHUD::BeginPlay()
{
	if (IsValid(_mainViewClass))
	{
		_mainView = CreateWidget<UTestUserWidget>(GetWorld(), _mainViewClass);
		if (_mainView)
		{
			_mainView->AddToViewport();
		}
	}
}
