// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Main/MainView.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "Components/TextBlock.h"

void UMainView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_Txt_ComboKeys = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_ComboKeys")));

	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::ComboStepExecute, this, TEXT("Rec_ComboStepExecute"));
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::ComboResumeExecute, this, TEXT("Rec_ComboResumeExecute"));
}

void UMainView::Rec_ComboStepExecute(TArray<FKey>& InComboMatchKeys)
{
	// for (auto tKey : InComboMatchKeys)
	// {
	// 	WVLogI(TEXT("%s"), *(tKey.ToString()));
	// }

	if (_Txt_ComboKeys)
	{
		FString comboKeyDesc = TEXT("");
		if (InComboMatchKeys.Contains(EKeys::Gamepad_FaceButton_Top))
		{
			comboKeyDesc = TEXT("轻格");
	
			if (InComboMatchKeys.Contains(EKeys::Gamepad_LeftX))
			{
				comboKeyDesc = TEXT("横") + comboKeyDesc;
			}
			else if (InComboMatchKeys.Contains(EKeys::Gamepad_LeftY))
			{
				// comboKeyDesc = TEXT("前") + comboKeyDesc;
				// comboKeyDesc = TEXT("后") + comboKeyDesc;
			}
		}
		else if (InComboMatchKeys.Contains(EKeys::Gamepad_FaceButton_Right))
		{
			comboKeyDesc = TEXT("重格");

			if (InComboMatchKeys.Contains(EKeys::Gamepad_LeftX))
			{
				comboKeyDesc = TEXT("横") + comboKeyDesc;
			}
			else if (InComboMatchKeys.Contains(EKeys::Gamepad_LeftY))
			{
				// comboKeyDesc = TEXT("前") + comboKeyDesc;
				// comboKeyDesc = TEXT("后") + comboKeyDesc;
			}
		}

		auto txtComboKeyDesc = _Txt_ComboKeys->GetText();
		auto tStr = FString::Printf(TEXT("%s[%s]"), *(txtComboKeyDesc.ToString()), *(comboKeyDesc));
		_Txt_ComboKeys->SetText(FText::FromString(tStr));
	}
}

void UMainView::Rec_ComboResumeExecute()
{
	_Txt_ComboKeys->SetText(FText::FromString(TEXT("")));
}
