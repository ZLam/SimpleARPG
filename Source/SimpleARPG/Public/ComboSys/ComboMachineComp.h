// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboMachineComp.generated.h"

class UComboNode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API UComboMachineComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UComboNode> Class_ComboNode;

	UFUNCTION()
	bool IsRunning() { return _CurNode != _EntryNode; }

	UFUNCTION()
	void Start();

	UFUNCTION()
	void Step();

	UFUNCTION()
	void Resume();

protected:
	UPROPERTY()
	UComboNode *_EntryNode;

	UPROPERTY()
	UComboNode *_CurNode;

public:
	// Sets default values for this component's properties
	UComboMachineComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
