// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/ScrollableGrid.h"

#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UScrollableGrid::NativeOnInitialized()
{
	_cellsExtraLines *= 2;

	if (!_useCellsToShapeGrid && !_fillGridWithCells)
	{		
		if (_gridOrientation == EGridOrientation::VERTICAL)
		{
			AdjustCellsCount(_columns, _cellSize.X, _gridPadding.Left, _gridDimensions.X - _gridPadding.Right,
				FVector2D{_cellLeftMargin, _cellRightMargin});
		}
		else
		{
			AdjustCellsCount(_rows, _cellSize.Y, _gridPadding.Top, _gridDimensions.Y - _gridPadding.Bottom,
				FVector2D{_cellTopMargin, _cellBottomMargin});
		}
	}
	
	Super::NativeOnInitialized();
}

void UScrollableGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
	int& currentYPosition)
{	
	Super::CreateHorizontalGrid(world, minXBounds, currentXPosition, currentYPosition);
	
	for (int i = 0; i < _cellsExtraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _columns; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);	
			newLine.Add(cell);
			currentXPosition += _cellLeftMargin + _cellSize.X + _cellRightMargin;
		}
		_cellGrid.Add(newLine);
		currentXPosition = minXBounds + _cellLeftMargin;
		currentYPosition += _cellTopMargin + _cellSize.Y + _cellBottomMargin;
	}
}

void UScrollableGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition, const int& minYBounds,
	int& currentYPosition)
{	
	Super::CreateVerticalGrid(world, currentXPosition, minYBounds, currentYPosition);

	for (int i = 0; i < _cellsExtraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);
			newLine.Add(cell);
			currentYPosition += _cellTopMargin + _cellSize.Y + _cellBottomMargin;	
		}
		_cellGrid.Add(newLine);
		currentYPosition = minYBounds + _cellTopMargin;
		currentXPosition += _cellLeftMargin + _cellSize.X + _cellRightMargin;
	}
}
