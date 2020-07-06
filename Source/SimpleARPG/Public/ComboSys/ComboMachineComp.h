// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboMachineComp.generated.h"

class UComboNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallback_ComboMachine);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API UComboMachineComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UComboNode> Class_ComboNode;

	UFUNCTION(BlueprintPure)
	bool IsRunning() { return _CurNode != _EntryNode; }

	UFUNCTION(BlueprintCallable)
	bool Start();

	UFUNCTION(BlueprintCallable)
	bool Step();

	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintPure)
	UComboNode* GetCurNode();

	bool _Step();

protected:
	UPROPERTY(VisibleAnywhere)
	UComboNode *_EntryNode;

	UPROPERTY(VisibleAnywhere)
	UComboNode *_CurNode;

	UPROPERTY(VisibleAnywhere)
	FString _CurComboName_AI;

	UPROPERTY(VisibleAnywhere)
	int32 _CurComboActionIndex;

	UFUNCTION(BlueprintNativeEvent)
	void SetupCurCombo_AI();

	UFUNCTION()
	void ClearCurCombo_AI();

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
