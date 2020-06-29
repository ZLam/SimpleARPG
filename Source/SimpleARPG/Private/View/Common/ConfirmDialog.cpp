// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Common/ConfirmDialog.h"

void UConfirmDialog::Confirm_Implementation()
{
	if (Callback_Confirm.IsBound())
	{
		Callback_Confirm.Execute();
	}
}

void UConfirmDialog::SetDesc_Implementation(const FText& txt)
{
	
}
