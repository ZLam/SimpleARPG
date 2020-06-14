// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputCoreTypes.h"
#include "ComboNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLEARPG_API UComboNode : public UObject
{
	GENERATED_BODY()
public:
	UComboNode();
	
	UFUNCTION()
	void InitData(const TArray<FKey> &InMatchKeys, const FString &InComboName, const int32 &InComboIndex);

	UFUNCTION(BlueprintPure)
	UComboNode* GetChild(const TArray<FKey> &InMatchKeys);

	UFUNCTION(BlueprintCallable)
	void AddChild(UComboNode *InNode);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<UComboNode*>& GetChildren() { return _Children; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetComboName() { return _ComboName; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetComboIndex() { return _ComboIndex; }

	UFUNCTION(BlueprintPure)
	AActor* GetOwner();

	//自定义条件
	UFUNCTION(BlueprintNativeEvent)
	bool Condition();

	//自定义执行
	UFUNCTION(BlueprintNativeEvent)
	void Do();

protected:
	UPROPERTY()
	TArray<UComboNode*> _Children;
	
	UPROPERTY()
	TMap<FKey, bool> _MatchKeysMap;

	UPROPERTY(VisibleAnywhere)
	FString _ComboName;

	UPROPERTY(VisibleAnywhere)
	int32 _ComboIndex;
};
