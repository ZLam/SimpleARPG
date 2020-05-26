// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/WVModelBase.h"
#include "EventSys/WVEventDispatcher.h"

UWVModelBase::UWVModelBase()
{
	UE_LOG(LogTemp, Display, TEXT("===UWVModelBase::UWVModelBase==="))
}

UWVModelBase::~UWVModelBase()
{
	UE_LOG(LogTemp, Display, TEXT("===UWVModelBase::~UWVModelBase==="))
}

void UWVModelBase::OnEnter()
{
	
}

void UWVModelBase::OnExit()
{
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}
