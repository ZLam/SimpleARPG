// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockTargetComp.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API ULockTargetComp : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FLockTarget_Delegate_Distance, float, distance);

	FLockTarget_Delegate_Distance Callback_Distance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> Class_TargetPoint;

	UPROPERTY(EditDefaultsOnly)
	FVector TargetPointRelativePos;

	UPROPERTY(EditDefaultsOnly)
	FVector2D TargetPointDrawSize;
	
	UFUNCTION(BlueprintCallable)
	void LockTarget(AActor *target);

	UFUNCTION(BlueprintCallable)
	void UnlockTarget();

	UFUNCTION(BlueprintPure)
	bool IsLocking();

protected:
	UPROPERTY(VisibleAnywhere)
	AActor *_CurTarget;

	void _UpdateLocking(float dt);

	void _AddTargetPoint();

	void _DelTargetPoint();

public:	
	// Sets default values for this component's properties
	ULockTargetComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
