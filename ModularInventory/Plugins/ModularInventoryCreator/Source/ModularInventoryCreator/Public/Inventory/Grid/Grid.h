// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryWidgetBase.h"
#include "Grid.generated.h"

class UTestDataSource;
class ATestDataSource;
class IIGridDataSource;
class UCanvasPanelSlot;
class USizeBoxSlot;
class USizeBox;
class UCanvasPanel;
class UGridPanel;
class UCell;
/**
 * 
 */

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

UCLASS(Abstract)
class MODULARINVENTORYCREATOR_API UGrid : public UInventoryWidgetBase
{
	friend class FGridEditor;
	
public:

#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void SetGridDataSource(UTestDataSource* gridDataSource);
	
	void InitGrid();

	void CreateBoundaries(const TObjectPtr<UWorld>& world);

	void AdjustBoundaries(const bool isOrientationVertical, int& minXBounds, const int& maxXBounds, int& currentXPosition,
		int& minYBounds, const int& maxYBounds, int& currentYPosition);

	static void AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	static void AdjustCellsCountWithClamp(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition, int& currentYPosition);
	
	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition,	const int& minYBounds, int& currentYPosition);

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> _sizeBoxSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> _sizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> _canvas = nullptr;

	#pragma region Layout

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	FVector2D _gridPivot;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	bool _useCellsToShapeGrid;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FVector2D _gridDimensions;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FMargin _gridPadding;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridOrientation _gridOrientation;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	bool _fillGridWithCells;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridVerticalAlignment _gridVerticalAlignment;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	EGridHorizontalAlignment _gridHorizontalAlignment;

	#pragma endregion

	TScriptInterface<IIGridDataSource> _gridDataSource = nullptr;	

	#pragma region Cells

	UPROPERTY(EditAnywhere, Category = "Grid|Cells")
	bool _useCellClassSizes;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	int _columns;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	int _rows;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells",meta = (ClampMin = 1, UIMin = 1))
	FVector2D _cellSize;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float _cellTopMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float _cellLeftMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float _cellRightMargin;

	UPROPERTY(EditAnywhere, Category = "Grid|Cells|Margins", meta = (ClampMin = 0, UIMin = 0))
	float _cellBottomMargin;

	TArray<TArray<TObjectPtr<UCell>>> _cellGrid;

	#pragma endregion

private:
	
	GENERATED_BODY()
};