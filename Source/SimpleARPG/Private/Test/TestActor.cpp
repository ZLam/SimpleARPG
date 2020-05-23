// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestActor.h"
#include "Engine/World.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATestActor::ATestActor():
TmpStr(TEXT("I am TestActor"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* comp_scene = CreateDefaultSubobject<USceneComponent>(TEXT("comp_scene"));
	RootComponent = comp_scene;

	comp_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("comp_staticMesh"));
	comp_staticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	// WVLogI(TEXT("info info info"))
	// WVLogW(TEXT("warning warning warning"))
	// // WVLogF("fatal fatal fatal")
	// WVLogE(TEXT("err err err"))

	// WVLogI(TEXT("ATestActor Class Begin"))
	// WVLogI(TEXT("%p"), ATestActor::StaticClass())
	// WVLogI(TEXT("%p"), ATestActor::StaticClass()->GetDefaultObject())
	// WVLogI(TEXT("%p"), ATestActor::GetClass())
	// WVLogI(TEXT("%p"), ATestActor::GetClass()->GetDefaultObject())
	// WVLogI(TEXT("ATestActor Class End"))

	// GetWorld()->SpawnActor(ATestActor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	// GetWorld()->SpawnActor(ATestActor2::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);

	auto sign = WVEventSignature(TEXT("Haha"), TEXT("TestEvent"));
	UWVEventDispatcher::GetInstance()->AddListener(sign, this, TEXT("TestEvent"));

	sign = WVEventSignature(TEXT("Haha"), TEXT("TestEvent_Str"));
	UWVEventDispatcher::GetInstance()->AddListener(sign, this, TEXT("TestEvent_Str"));

	sign = WVEventSignature(TEXT("Haha"), TEXT("TestEvent_Delegate"));
	FWVEventDelegate_One tDelegate;
	tDelegate.BindUFunction(this, TEXT("TestEvent_Delegate"));
	UWVEventDispatcher::GetInstance()->AddListener(sign, this, tDelegate);
}

void ATestActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// auto sign = WVEventSignature(TEXT("Haha"), TEXT("TestEvent"));
	// UWVEventDispatcher::GetInstance()->RemoveListener(sign, this);


	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
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

void ATestActor::TestEvent_Str(FString &str)
{
	WVLogI(TEXT("ATestActor::TestEvent_Str_%s"), *str);
	str = TEXT("change la la la");
	WVLogI(TEXT("ATestActor::TestEvent_Str_%s"), *str);
}

void ATestActor::TestEvent_Delegate(FWVEventDelegateParams_One params)
{
	if (params.dataPtr)
	{
		WVLogI(TEXT("ATestActor::TestEvent_Delegate_%s"), *(*(FString*)(params.dataPtr)));
		(*(FString*)(params.dataPtr)) = TEXT("change he he he");
		WVLogI(TEXT("ATestActor::TestEvent_Delegate_%s"), *(*(FString*)(params.dataPtr)));
	}
}
