// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/Grid.h"

#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/SizeBox.h"

void UGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TObjectPtr<UWorld> world = GetWorld();

	checkf(world, TEXT("Unable to get a reference of the world"));

	_sizeBoxSlot = Cast<UCanvasPanelSlot>(_sizeBox->Slot);

	checkf(_sizeBoxSlot, TEXT("_sizeBox needs to be a child of the root widget"));

	if (_useCellClassSizes)
	{
		TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _cellClass);		
		_cellSize = cell->GetCellSize();
	}

	if (_useCellsToShapeGrid)
	{
		_gridDimensions = FVector2D(_gridPadding.Left + _cellsCount.row * (_cellSize.X + _cellsSpace) + _gridPadding.Right,
			_gridPadding.Top + _cellsCount.column * (_cellSize.Y + _cellsSpace) + _gridPadding.Bottom);
	}

	_cellsSpace /= 2;

	InitGrid(world);
}

void UGrid::InitGrid(const TObjectPtr<UWorld>& world)
{	
	_sizeBox->SetRenderTranslation(_gridPivot);

	_sizeBoxSlot->SetSize(_gridDimensions);

	CreateBoundaries(world);	
}

void UGrid::CreateBoundaries(const TObjectPtr<UWorld>& world)
{
	int minXBounds = _gridPadding.Left;
	int maxXBounds = _gridDimensions.X - _gridPadding.Right;
	int currentXPosition = minXBounds + _cellsSpace;

	int minYBounds = _gridPadding.Top;
	int maxYBounds = _gridDimensions.Y - _gridPadding.Bottom;
	int currentYPosition = minYBounds + _cellsSpace;

	const bool isOrientationVertical = _gridOrientation == EGridOrientation::VERTICAL;

	if (!_useCellsToShapeGrid && !_fillGridWithCells)
	{
		AdjustBoundaries(isOrientationVertical, minXBounds, maxXBounds, currentXPosition, minYBounds, maxYBounds, currentYPosition);
	}
	
	AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
	AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);
	
	if (isOrientationVertical)
	{
		CreateVerticalGrid(world, currentXPosition, minYBounds, currentYPosition);
		return;
	}
	
	CreateHorizontalGrid(world, minXBounds, currentXPosition, currentYPosition);
}

void UGrid::AdjustBoundaries(const bool isOrientationVertical, int& minXBounds, const int& maxXBounds, int& currentXPosition,
	int& minYBounds, const int& maxYBounds, int& currentYPosition)
{
	if (isOrientationVertical)
	{		
		const int& availableVerticalSpace = maxYBounds - minYBounds;	

		if (_gridVerticalAlignment == EGridVerticalAlignment::FILL)
		{
			const int& cellsPerColumn = _cellsCount.column;
			
			const float& spaceOccupiedByCells = cellsPerColumn * _cellSize.Y;

			const float& finalAvailableVerticalSpace = availableVerticalSpace - spaceOccupiedByCells;

			if (finalAvailableVerticalSpace < 0)
			{
				_cellsSpace = 0;
				AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
				AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);
				currentYPosition = minYBounds;
				return;
			}

			_cellsSpace = finalAvailableVerticalSpace / cellsPerColumn;
			_cellsSpace /= 2;
			AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
			currentYPosition = minYBounds + _cellsSpace;
		
			return;
		}

		AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);		
		AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
		
		const int& cellsPerColumn = _cellsCount.column;

		if (_gridVerticalAlignment == EGridVerticalAlignment::CENTER)
		{
			int halfCells = FMath::Floor(cellsPerColumn / 2);

			minYBounds = availableVerticalSpace / 2 - halfCells * (_cellSize.Y + _cellsSpace * 2) + (_cellsSpace + _cellSize.Y / 2);
		}

		if (_gridVerticalAlignment == EGridVerticalAlignment::BOTTOM)
		{
			minYBounds = maxYBounds - cellsPerColumn * (_cellSize.Y + _cellsSpace * 2);
		}

		currentYPosition = minYBounds + _cellsSpace;

		return;
	}
	
	const int& availableHorizontalSpace = maxXBounds - minXBounds;
		
	if (_gridHorizontalAlignment == EGridHorizontalAlignment::FILL)
	{
		const int& cellsPerRow = _cellsCount.row;

		const float& spaceOccupiedByCells = cellsPerRow * _cellSize.X;

		const float& finalAvailableHorizontalSpace = availableHorizontalSpace - spaceOccupiedByCells;  

		if (finalAvailableHorizontalSpace < 0)
		{
			_cellsSpace = 0;
			AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);
			AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
			currentXPosition = minXBounds;
			return;
		}

		_cellsSpace = finalAvailableHorizontalSpace / cellsPerRow;
		_cellsSpace /= 2;
		AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);
		currentXPosition = minXBounds + _cellsSpace;

		return;
	}

	AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds);
	AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds);
		
	const int& cellsPerRow = _cellsCount.row;

	if (_gridHorizontalAlignment == EGridHorizontalAlignment::CENTER)
	{
		int halfCells = FMath::Floor(cellsPerRow / 2);

		minXBounds = availableHorizontalSpace / 2 - halfCells * (_cellSize.X + _cellsSpace * 2) + (_cellsSpace + _cellSize.X / 2);
	}

	if (_gridHorizontalAlignment == EGridHorizontalAlignment::RIGHT)
	{
		minXBounds = maxXBounds - cellsPerRow * (_cellSize.X + _cellsSpace * 2);
	}

	currentXPosition = minXBounds + _cellsSpace;
}

void UGrid::AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds) const
{
	int currentCheckPosition = minBounds;

	/*if (_fillGridWithCells)
	{	
		while (currentCheckPosition < maxBounds)
		{
			cellsPerLine++;
			currentCheckPosition += cellSize + _cellsSpace * 2;
		}
		return;
	}*/
	
	const int currentCellsPerLine = cellsPerLine;
	
	cellsPerLine = 0;
	
	while (currentCheckPosition < maxBounds && cellsPerLine < currentCellsPerLine)
	{
		cellsPerLine++;
		currentCheckPosition += cellSize + _cellsSpace * 2;
	}
}

void UGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition, const int& minYBounds, int& currentYPosition)
{
	for (int i = 0; i < _cellsCount.row; ++i)
	{		
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _cellsCount.column; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);
			newLine.Add(cell);
			currentYPosition += _cellSize.Y + _cellsSpace * 2;
		}
		_cellGrid.Add(newLine);
		currentYPosition = minYBounds + _cellsSpace;
		currentXPosition += _cellSize.X + _cellsSpace * 2;
	}
}

void UGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition, int& currentYPosition)
{		
	for (int i = 0; i < _cellsCount.column; ++i)
	{		
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _cellsCount.row; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);
			newLine.Add(cell);
			currentXPosition += _cellSize.X + _cellsSpace * 2;
		}
		_cellGrid.Add(newLine);
		currentXPosition = minXBounds + _cellsSpace;
		currentYPosition += _cellSize.Y + _cellsSpace * 2;
	}
}

#if WITH_EDITOR

void UGrid::OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs)
{
	Super::OnDesignerChanged(EventArgs);
}

void UGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif