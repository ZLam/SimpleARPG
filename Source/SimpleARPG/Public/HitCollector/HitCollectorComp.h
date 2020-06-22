// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionQueryParams.h"
#include "HitCollectorComp.generated.h"

class UShapeComponent;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitCollectorCallback, const FHitResult&, HitResult, AActor*, Causer, UActorComponent*, CauserComp);

USTRUCT()
struct FHitOnceInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	TSet<UObject*> Hiteds;

	bool IsHited(UObject *obj)
	{
		return Hiteds.Contains(obj);
	}

	void Hited(UObject *obj)
	{
		Hiteds.Add(obj);
	}

	void Reset()
	{
		Hiteds.Empty();
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API UHitCollectorComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FHitCollectorCallback HitCallback;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsEnable() { return _bEnable; }
	
	UFUNCTION(BlueprintCallable)
	void Enable();

	UFUNCTION(BlueprintCallable)
	void Disable();

	UFUNCTION(BlueprintPure)
	ACharacter* GetOwnerCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	bool _bEnable;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> _ColliderTags;

	UPROPERTY(VisibleAnywhere)
	TArray<UShapeComponent*> _Colliders;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ECollisionChannel>> _HitChannels;

	UPROPERTY(EditDefaultsOnly)
	bool _bHitOnce;

	TArray<FVector> _LastPosArr;

	FCollisionObjectQueryParams _ChannelsQueryParams;

	FCollisionQueryParams _CollisionQueryParams;
	
	TMap<UShapeComponent*, TSharedPtr<FHitOnceInfo>> _HitOnceMap;
	// TMap<UShapeComponent*, FHitOnceInfo> _HitOnceMap;		//甘样唔得的，用果时Empty唔到。具体要睇睇源码先知了

	UFUNCTION()
	void HandleHit();

	UFUNCTION()
	void ResetLastPosArr();

	UFUNCTION()
	void ResetHitOnceMap();

	ACharacter* _GetOwnerCharacter(AActor* InOwner);

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
