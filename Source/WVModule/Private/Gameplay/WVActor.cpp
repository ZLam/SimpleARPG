// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WVActor.h"
#include "EventSys/WVEventDispatcher.h"

// Sets default values
AWVActor::AWVActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWVActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWVActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AWVActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

