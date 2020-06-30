// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WVCharacter.h"
#include "EventSys/WVEventDispatcher.h"

// Sets default values
AWVCharacter::AWVCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWVCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWVCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AWVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWVCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

