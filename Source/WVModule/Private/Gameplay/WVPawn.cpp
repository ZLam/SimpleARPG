// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WVPawn.h"
#include "EventSys/WVEventDispatcher.h"

// Sets default values
AWVPawn::AWVPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWVPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWVPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AWVPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWVPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

