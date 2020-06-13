// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputCoreTypes.h"
#include "ComboNode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UComboNode : public UObject
{
	GENERATED_BODY()
public:
	UComboNode();
	
	UFUNCTION()
	void InitData(const TArray<FKey> &InMatchKeys, const FString &InComboName, const int32 &InComboIndex);

	UFUNCTION()
	UComboNode* GetChild(const TArray<FKey> &InMatchKeys);

	UFUNCTION()
	void AddChild(UComboNode *InNode);

	UFUNCTION()
	FORCEINLINE TArray<UComboNode*>& GetChildren() { return _Children; }

	UFUNCTION()
	FORCEINLINE FString GetComboName() { return _ComboName; }

	UFUNCTION()
	FORCEINLINE int32 GetComboIndex() { return _ComboIndex; }

	//自定义条件
	UFUNCTION(BlueprintNativeEvent)
	bool Condition();

	//自定义执行
	UFUNCTION(BlueprintNativeEvent)
	void Do();

protected:
	UPROPERTY()
	TArray<UComboNode*> _Children;
	
	UPROPERTY(VisibleAnywhere)
	TMap<FKey, bool> _MatchKeysMap;

	UPROPERTY(VisibleAnywhere)
	FString _ComboName;

	UPROPERTY(VisibleAnywhere)
	int32 _ComboIndex;
};
