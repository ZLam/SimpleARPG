// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestActor.h"
#include "WVModule/Public/Logger/WVLog.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	WVLOG_INFO("info info info")
	WVLOG_WARNING("warning warning warning")
	// WVLOG_FATAL("fatal fatal fatal")
	WVLOG_ERROR("err err err")
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

