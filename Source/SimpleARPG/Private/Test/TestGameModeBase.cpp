// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestGameModeBase.h"
#include "SimpleARPG/Public/Test/TestHUD.h"
#include "SimpleARPG/Public/Test/TestPlayerController.h"

ATestGameModeBase::ATestGameModeBase()
{
	PlayerControllerClass = ATestPlayerController::StaticClass();
	HUDClass = ATestHUD::StaticClass();
}
