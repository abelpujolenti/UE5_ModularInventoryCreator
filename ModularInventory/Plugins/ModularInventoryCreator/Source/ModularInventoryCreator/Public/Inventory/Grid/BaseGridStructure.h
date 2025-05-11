// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DataSources/BaseItemDataSource.h"
#include "Inventory/Scroll/BaseScrollBarProperties.h"
#include "BaseGridStructure.generated.h"

UENUM(BlueprintType)
enum class EGridOrientation : uint8
{
	VERTICAL UMETA(DisplayName = "Vertical"),
	HORIZONTAL UMETA(DisplayName = "Horizontal")
};

UENUM(BlueprintType)
enum class EGridVerticalAlignment : uint8
{
	FILL UMETA(DisplayName = "Fill"),
	TOP UMETA(DisplayName = "Top"),
	CENTER UMETA(DisplayName = "Center"),
	BOTTOM UMETA(DisplayName = "Bottom")
};

UENUM(BlueprintType)
enum class EGridHorizontalAlignment : uint8
{
	FILL UMETA(DisplayName = "Fill"),
	LEFT UMETA(DisplayName = "Left"),
	CENTER UMETA(DisplayName = "Center"),
	RIGHT UMETA(DisplayName = "Right")
};

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UBaseGridStructure : public UObject
{
	friend class FGridStructureEditor;

public:

	#pragma region Scroll

	UPROPERTY(EditAnywhere, Category = "Grid|Scroll")
	bool isScrollable;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Scroll", meta = (ClampMin = 1, UIMin = 1))
	int extraLines;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Scroll", meta = (ClampMin = 0.1f, UIMin = 0.1f))
	float scrollDistanceMultiplier;

	UPROPERTY(EditAnywhere, Category = "Grid|Scroll")
	bool hasScrollBar;

	UPROPERTY(EditAnywhere, Category = "Grid|Scroll")
	TSubclassOf<UBaseScrollBarProperties> scrollBarClass = nullptr;

	#pragma endregion

	#pragma region Layout

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	FVector2D gridPivot;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	bool useCellsToShapeGrid;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FVector2D gridDimensions;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FMargin gridPadding;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridOrientation gridOrientation;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	bool fillGridWithCells;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridVerticalAlignment gridVerticalAlignment;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridHorizontalAlignment gridHorizontalAlignment;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	TObjectPtr<UTexture2D> background = nullptr;

	#pragma endregion

	#pragma region Items

	UPROPERTY(EditAnywhere, Category = "Grid|Cells")
	TSubclassOf<UBaseItemDataSource> gridDataSourceClass = nullptr;

	#pragma endregion

	#pragma region Cells	

	UPROPERTY(EditAnywhere, Category = "Grid|Cells")
	bool useCellClassSizes;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	int columns;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	int rows;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells",meta = (ClampMin = 1, UIMin = 1))
	FVector2D cellSize;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float cellTopMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float cellLeftMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float cellRightMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float cellBottomMargin;

	#pragma endregion

private:
	GENERATED_BODY()
};
