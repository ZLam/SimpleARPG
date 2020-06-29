// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Common/ChooseNumDialog.h"
#include "Components/Slider.h"

void UChooseNumDialog::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_Slider_Num = Cast<USlider>(GetWidgetFromName(TEXT("Slider_Num")));
}

void UChooseNumDialog::Confirm_Implementation()
{
	if (Callback_Confirm.IsBound())
	{
		float num = _Slider_Num->GetValue();
		Callback_Confirm.Execute(num);
	}
}

void UChooseNumDialog::SetMin_Implementation(float num)
{

}

void UChooseNumDialog::SetMax_Implementation(float num)
{

}

void UChooseNumDialog::SetCur_Implementation(float num)
{
	
}
