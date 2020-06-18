// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionQueryParams.h"
#include "HitCollectorComp.generated.h"

class UShapeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitCollectorCallback, const FHitResult&, HitResult, AActor*, Causer, UActorComponent*, CauserComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API UHitCollectorComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FHitCollectorCallback HitCallback;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsEnable() { return _bEnable; }
	
	UFUNCTION(BlueprintCallable)
	void Enable();

	UFUNCTION(BlueprintCallable)
	void Disable();

protected:
	UPROPERTY(VisibleAnywhere)
	bool _bEnable;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> _ColliderTags;

	UPROPERTY(VisibleAnywhere)
	TArray<UShapeComponent*> _Colliders;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ECollisionChannel>> _HitChannels;

	TArray<FVector> _LastPosArr;

	FCollisionObjectQueryParams _ChannelsQueryParams;

	FCollisionQueryParams _CollisionQueryParams;

	UFUNCTION()
	void HandleHit();

	UFUNCTION()
	void ResetLastPosArr();

public:	
	// Sets default values for this component's properties
	UHitCollectorComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
