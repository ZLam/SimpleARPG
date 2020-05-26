﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "WVModule/Public/Model/WVModelMgr.h"
#include "WVModule/Public/View/WVViewCreatorMgr.h"
#include "Model/Model_Player.h"
#include "View/Item/ItemCreator.h"

void UMyGameInstance::Init()
{
	Super::Init();


	/**
	 * 注冊model
	 */
	UWVModelMgr::GetInstance()->RegistModel<UModel_Player>(EWVModelName::Player);

	/**
	 * 注冊creator
	 */
	UWVViewCreatorMgr::GetInstance()->RegistCreator<UItemCreator>(TEXT("ItemCreator"));
}
