// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/GridStructure.h"
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

protected:

	virtual void InitializeGrid(const UGridStructure& gridStructure);

	virtual void InstantiateGrid();
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Content")
	virtual void SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource);
	
	void InitGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource);

	virtual void InstantiateWidgets();

	void CreateBoundaries(const TObjectPtr<UWorld>& world);
	
	void AdjustBoundaries(const bool isOrientationVertical, int& minXBounds, const int& maxXBounds, int& initialXPosition,
		int& minYBounds, const int& maxYBounds, int& initialYPosition);
	
	static void AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	static void AdjustCellsCountWithClamp(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds, const FVector2D& margins);
	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world) PURE_VIRTUAL(UBaseGrid::CreateHorizontalGrid);
	
	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world) PURE_VIRTUAL(UBaseGrid::CreateVerticalGrid);

	void FillCell(TObjectPtr<UCell> cell, int index) const;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> _sizeBoxSlot = nullptr;

	UPROPERTY()
	TObjectPtr<USizeBox> _sizeBox = nullptr;

	UPROPERTY()
	TObjectPtr<UCanvasPanel> _canvas = nullptr;

	#pragma region Layout

	UPROPERTY()
	FVector2D _gridPivot;

	UPROPERTY()
	bool _useCellsToShapeGrid;

	UPROPERTY()
	FVector2D _gridDimensions;

	UPROPERTY()
	FMargin _gridPadding;

	UPROPERTY()
	EGridOrientation _gridOrientation;

	UPROPERTY()
	bool _fillGridWithCells;

	UPROPERTY()
	EGridVerticalAlignment _gridVerticalAlignment;

	UPROPERTY()
	EGridHorizontalAlignment _gridHorizontalAlignment;

	UPROPERTY()
	TObjectPtr<UTexture2D> _background;

	#pragma endregion

	UPROPERTY()
	TSubclassOf<UBaseItemDataSource> _gridDataSourceClass = nullptr;

	TScriptInterface<IIGridItemDataSource> _gridDataSource = nullptr;	

	#pragma region Cells

	UPROPERTY()
	bool _useCellClassSizes;

	UPROPERTY()
	int _columns;

	UPROPERTY()
	int _rows;

	UPROPERTY()
	FVector2D _cellSize;

	UPROPERTY()
	float _cellTopMargin;

	UPROPERTY()
	float _cellLeftMargin;

	UPROPERTY()
	float _cellRightMargin;

	UPROPERTY()
	float _cellBottomMargin;

	TArray<TArray<TObjectPtr<UCell>>> _cellGrid;

	#pragma endregion

	std::function<void()> _fillFunction;

private:
	GENERATED_BODY()
};
