// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestActor.h"
#include "Engine/World.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"

// Sets default values
ATestActor::ATestActor():
TmpStr(TEXT("I am TestActor"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	// WVLogI(TEXT("info info info"))
	// WVLogW(TEXT("warning warning warning"))
	// // WVLogF("fatal fatal fatal")
	// WVLogE(TEXT("err err err"))

	WVLogI(TEXT("ATestActor Class Begin"))
	WVLogI(TEXT("%p"), ATestActor::StaticClass())
	WVLogI(TEXT("%p"), ATestActor::StaticClass()->GetDefaultObject())
	WVLogI(TEXT("%p"), ATestActor::GetClass())
	WVLogI(TEXT("%p"), ATestActor::GetClass()->GetDefaultObject())
	WVLogI(TEXT("ATestActor Class End"))

	// GetWorld()->SpawnActor(ATestActor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	// GetWorld()->SpawnActor(ATestActor2::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);

	auto sign = WVEventSignature(TEXT("Haha"), TEXT("TestEvent"));
	UWVEventDispatcher::GetInstance()->AddListener(sign, this, TEXT("TestEvent"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::TestEvent()
{
	WVLogI(TEXT("ATestActor::TestEvent"))
}
