#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WVConfigCenter.generated.h"

class UTexture2D;

/**
 * DataTable配置枚举
 */
UENUM(BlueprintType)
enum class EWVConfigName : uint8
{
	Item		UMETA(DisplayName = "Item"),	//道具
	Shop		UMETA(DisplayName = "Shop"),	//商店
	MAX,
};

/**
 * gameplay道具类型枚举
 */
UENUM(BlueprintType)
enum class EWVItemType : uint8
{
	Money		UMETA(DisplayName = "Money"),	//货币
	Item		UMETA(DisplayName = "Item"),	//道具
	MAX,
};

/**
 * gameplay道具类型枚举
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
 * DataTable配置表头
 */

/**
 * 道具配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ItemRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWVConfig_ItemRow():
	id(0),
	type(EWVItemType::Money),
	quality(EWVQualityType::White),
	name(TEXT("")),
	desc(TEXT("")),
	icon(nullptr)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVItemType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVQualityType quality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> icon;
};

/**
 * 商店配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ShopRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWVConfig_ShopRow():
	idArr()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> idArr;
};

/**
 * 商店商品配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ShopGoodsRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWVConfig_ShopGoodsRow():
	itemId(0),
	costId(0)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 costId;
};
