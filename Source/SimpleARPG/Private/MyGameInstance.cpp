// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "WVModule/Public/Model/WVModelMgr.h"
#include "WVModule/Public/View/WVViewCreatorMgr.h"
#include "Model/Model_Player.h"
#include "View/Login/LoginCreator.h"
#include "View/Main/MainCreator.h"
#include "View/Item/ItemCreator.h"

//@TEST begin
#include "Test/TestCreator.h"
//@TEST end

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
	UWVViewCreatorMgr::GetInstance()->RegistCreator<ULoginCreator>(FString(TEXT("LoginCreator")));
	UWVViewCreatorMgr::GetInstance()->RegistCreator<UMainCreator>(FString(TEXT("MainCreator")));
	UWVViewCreatorMgr::GetInstance()->RegistCreator<UItemCreator>(FString(TEXT("ItemCreator")));
	//@TEST begin
	UWVViewCreatorMgr::GetInstance()->RegistCreator<UTestCreator>(FString(TEXT("TestCreator")));
	//@TEST end
}
