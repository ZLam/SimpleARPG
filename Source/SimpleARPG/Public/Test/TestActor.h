// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WVModule/Public/EventSys/WVEventDelegate.h"
#include "TestActor.generated.h"

UCLASS()
class SIMPLEARPG_API ATestActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	bool bTmp;
	UPROPERTY(VisibleAnywhere)
	FString TmpStr;
	UPROPERTY(VisibleAnywhere)
	AActor* TmpActor;
	UPROPERTY(VisibleAnywhere)
	UObject* TmpObj;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* comp_staticMesh;

	UFUNCTION()
	void TestEvent();

	UFUNCTION()
	void TestEvent_Str(FString &str);

	UFUNCTION()
	void TestEvent_Delegate(FWVEventDelegateParams_One params);
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Overridable function called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
