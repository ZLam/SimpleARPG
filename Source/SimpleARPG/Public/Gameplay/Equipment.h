// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class UHitCollectorComp;

UCLASS()
class SIMPLEARPG_API AEquipment : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UHitCollectorComp* GetHitCollectorComp() { return _Comp_HitCollector; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UHitCollectorComp *_Comp_HitCollector;
	
public:	
	// Sets default values for this actor's properties
	AEquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
