// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Equipment.h"
#include "HitCollector/HitCollectorComp.h"

// Sets default values
AEquipment::AEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	_Shake = nullptr;
	_Comp_HitCollector = CreateDefaultSubobject<UHitCollectorComp>(TEXT("Comp_HitCollector"));
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

