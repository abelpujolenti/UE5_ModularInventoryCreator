// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Grid/BaseGridStructure.h"
#include "Inventory/InventoryWidgetBase.h"
#include "BaseGrid.generated.h"

/**
 * 
 */

class UIGridItemDataSource;
class IIGridItemDataSource;
class UCanvasPanel;
class USizeBox;
class UCanvasPanelSlot;
class UCell;

UCLASS(Abstract)
class MODULARINVENTORYCREATOR_API UBaseGrid : public UInventoryWidgetBase
{
	friend class UFactoryGrid;
	
public:

#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual ~UBaseGrid() override = default;

protected:

	virtual void InitializeGrid(const UBaseGridStructure& gridStructure);

	virtual void InstantiateGrid();
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Content")
	virtual void SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource);
	
	void InitGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource);

	virtual void InstantiateWidgets() override;

	void CreateBoundaries(const TObjectPtr<UWorld>& world);
	
	void AdjustBoundaries(const bool isOrientationVertical, int& minXBounds, const int& maxXBounds, int& initialXPosition,
		int& minYBounds, const int& maxYBounds, int& initialYPosition);
	
	static void AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	static void AdjustCellsCountWithClamp(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world) PURE_VIRTUAL(UBaseGrid::CreateHorizontalGrid);
	
	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world) PURE_VIRTUAL(UBaseGrid::CreateVerticalGrid);

	void FillCell(TObjectPtr<UCell> cell, int index) const;

	#pragma region Layout

	TObjectPtr<UCanvasPanelSlot> _sizeBoxSlot = nullptr;
	TObjectPtr<USizeBox> _sizeBox = nullptr;
	TObjectPtr<UCanvasPanel> _canvas = nullptr;

	FVector2D _gridPivot;

	bool _useCellsToShapeGrid;

	FVector2D _gridDimensions;
	FMargin _gridPadding;
	EGridOrientation _gridOrientation;

	bool _fillGridWithCells;

	EGridVerticalAlignment _gridVerticalAlignment;
	EGridHorizontalAlignment _gridHorizontalAlignment;

	TObjectPtr<UTexture2D> _background;

	#pragma endregion

	TSubclassOf<UBaseItemDataSource> _gridDataSourceClass = nullptr;
	TScriptInterface<IIGridItemDataSource> _gridItemDataSource = nullptr;	

	#pragma region Cells

	bool _useCellClassSizes;

	int _columns;
	int _rows;

	FVector2D _cellSize;
	float _cellTopMargin;
	float _cellLeftMargin;
	float _cellRightMargin;
	float _cellBottomMargin;

	TArray<TArray<TObjectPtr<UCell>>> _cellGrid;

	#pragma endregion

	std::function<void()> _fillFunction;

private:
	GENERATED_BODY()
};
