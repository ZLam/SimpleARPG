// Fill out your copyright notice in the Description page of Project Settings.


#include "LockTargetSys/TargetPointWidget.h"
#include "WVModule/Public/Logger/WVLog.h"

void UTargetPointWidget::NativeOnInitialized()
{
	_curType = ETargetPointType::None;
	_red = GetWidgetFromName(TEXT("red"));
	_green = GetWidgetFromName(TEXT("green"));
	_yellow = GetWidgetFromName(TEXT("yellow"));
}

void UTargetPointWidget::Show(ETargetPointType type)
{
	if (type == _curType)
	{
		return;
	}
	_curType = type;
	_red->SetVisibility(_curType == ETargetPointType::Red ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_green->SetVisibility(_curType == ETargetPointType::Green ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_yellow->SetVisibility(_curType == ETargetPointType::Yellow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
