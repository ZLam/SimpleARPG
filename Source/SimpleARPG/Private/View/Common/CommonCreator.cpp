// Fill out your copyright notice in the Description page of Project Settings.


#include "View/Common/CommonCreator.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/View/WVViewMgr.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "View/Common/MakeSureDialog.h"
#include "View/Common/ConfirmDialog.h"
#include "View/Common/ChooseNumDialog.h"

void UCommonCreator::OnEnter()
{
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateMakeSureDialog, this, TEXT("Rec_OnCreateMakeSureDialog"));
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateConfirmDialog, this, TEXT("Rec_OnCreateConfirmDialog"));
	UWVEventDispatcher::GetInstance()->AddListener(EWVEventCategory::Inner, EWVEventName::CreateChooseNumDialog, this, TEXT("Rec_OnCreateChooseNumDialog"));
}

void UCommonCreator::Rec_OnCreateMakeSureDialog(const FWVParams_CreateMakeSureDialog &Params)
{
	auto dialog = UWVViewMgr::GetInstance()->PushDialog<UMakeSureDialog>(TEXT("WidgetBlueprint'/Game/View/Common/WidgetBP_MakeSureDialog.WidgetBP_MakeSureDialog_C'"));

	// WVLogI(TEXT("%d"), Params.Callback_IsOk.IsBound())
	// WVLogI(TEXT("%s_%s_%s"), *(Params.StrDesc.ToString()), *(Params.StrOk.ToString()), *(Params.StrCancel.ToString()))
	
	if (dialog)
	{
		dialog->Callback_IsOk = Params.Callback_IsOk;		
		dialog->SetDesc(Params.TxtDesc);
		dialog->SetOk(Params.TxtOk);
		dialog->SetCancel(Params.TxtCancel);
	}
}

void UCommonCreator::Rec_OnCreateConfirmDialog(const FWVParams_CreateConfirmDialog& Params)
{
	auto dialog = UWVViewMgr::GetInstance()->PushDialog<UConfirmDialog>(TEXT("WidgetBlueprint'/Game/View/Common/WidgetBP_ConfirmDialog.WidgetBP_ConfirmDialog_C'"));

	if (dialog)
	{
		dialog->Callback_Confirm = Params.Callback_Confirm;
		dialog->SetDesc(Params.TxtDesc);
	}
}

void UCommonCreator::Rec_OnCreateChooseNumDialog(const FWVParams_CreateChooseNumDialog& Params)
{
	auto dialog = UWVViewMgr::GetInstance()->PushDialog<UChooseNumDialog>(TEXT("WidgetBlueprint'/Game/View/Common/WidgetBP_ChooseNumDialog.WidgetBP_ChooseNumDialog_C'"));

	if (dialog)
	{
		dialog->Callback_Confirm = Params.Callback_ChooseNum;
		if (Params.MinNum <= Params.MaxNum)
		{
			dialog->SetMin(Params.MinNum);
			dialog->SetMax(Params.MaxNum);
			dialog->SetCur(Params.MinNum);
		}
		else
		{
			WVLogW(TEXT("MinNum > MaxNum !!!"))
		}
	}
}
