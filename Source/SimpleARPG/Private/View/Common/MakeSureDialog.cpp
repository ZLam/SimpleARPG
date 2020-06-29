// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Common/MakeSureDialog.h"

void UMakeSureDialog::SetDesc_Implementation(const FText& txt)
{
	
}

void UMakeSureDialog::SetOk_Implementation(const FText& txt)
{
	
}

void UMakeSureDialog::SetCancel_Implementation(const FText& txt)
{
	
}

void UMakeSureDialog::Confirm_Implementation()
{
	if (Callback_IsOk.IsBound())
	{
		Callback_IsOk.Execute(true);
	}
}

void UMakeSureDialog::Cancel_Implementation()
{
	if (Callback_IsOk.IsBound())
	{
		Callback_IsOk.Execute(false);
	}
}
