// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ActionCharacter.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

