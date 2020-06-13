#pragma once

#include "CoreMinimal.h"
#include "WVGameTypes.generated.h"

/**
 * gameplay 道具类型枚举
 */
UENUM(BlueprintType)
enum class EWVItemType : uint8
{
	Money		UMETA(DisplayName = "Money"),	//货币
	Item		UMETA(DisplayName = "Item"),	//道具
	MAX,
};

/**
 * gameplay 品质枚举
 */
UENUM(BlueprintType)
enum class EWVQualityType : uint8
{
	White		UMETA(DisplayName = "White"),
	Green		UMETA(DisplayName = "Green"),
	Blue		UMETA(DisplayName = "Blue"),
	Purple		UMETA(DisplayName = "Purple"),
	Orange		UMETA(DisplayName = "Orange"),
	MAX,
};

/**
 * gameplay InputBuffer各种匹配方式
 */
UENUM(BlueprintType)
enum class EWVInputMatchStyle : uint8
{
	OnlyLastTime		UMETA(DisplayName = "OnlyLastTime"),
};

/**
 * gameplay InputBuffer匹配会用到的数据
 */
USTRUCT(BlueprintType)
struct FWVInputMatchData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_LastTime;
};
