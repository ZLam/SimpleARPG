// Fill out your copyright notice in the Description page of Project Settings.


#include "View/WVViewCreatorBase.h"
#include "EventSys/WVEventDispatcher.h"

UWVViewCreatorBase::UWVViewCreatorBase()
{
	UE_LOG(LogTemp, Display, TEXT("===UWVViewCreatorBase::UWVViewCreatorBase==="))
}

UWVViewCreatorBase::~UWVViewCreatorBase()
{
	UE_LOG(LogTemp, Display, TEXT("===UWVViewCreatorBase::~UWVViewCreatorBase==="))
}

void UWVViewCreatorBase::OnEnter()
{

}

void UWVViewCreatorBase::OnExit()
{
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}
