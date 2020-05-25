// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/WVModelBase.h"
#include "EventSys/WVEventDispatcher.h"

void UWVModelBase::OnEnter()
{
	
}

void UWVModelBase::OnExit()
{
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}
