﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "View/WVViewBase.h"
#include "EventSys/WVEventDispatcher.h"

void UWVViewBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWVViewBase::NativeDestruct()
{
	Super::NativeDestruct();

	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}