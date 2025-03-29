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
		_gridDimensions = FVector2D(_gridPadding.Left + _columns * (_cellLeftMargin + _cellSize.X + _cellRightMargin) + _gridPadding.Right,
			_gridPadding.Top + _rows * (_cellTopMargin + _cellSize.Y + _cellBottomMargin) + _gridPadding.Bottom);
	}

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
	int currentXPosition = minXBounds + _cellLeftMargin;

	int minYBounds = _gridPadding.Top;
	int maxYBounds = _gridDimensions.Y - _gridPadding.Bottom;
	int currentYPosition = minYBounds + _cellTopMargin;

	const bool isOrientationVertical = _gridOrientation == EGridOrientation::VERTICAL;

	if (!_useCellsToShapeGrid)
	{
		if (_fillGridWithCells)
		{	
			AdjustCellsCount(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});
			AdjustCellsCount(_rows, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellTopMargin, _cellBottomMargin});
		}
		else
		{
			AdjustBoundaries(isOrientationVertical, minXBounds, maxXBounds, currentXPosition, minYBounds, maxYBounds, currentYPosition);
		}
	}
	
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
		AdjustCellsCountWithClamp(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});
		
		const int& availableVerticalSpace = maxYBounds - minYBounds;	

		if (_gridVerticalAlignment == EGridVerticalAlignment::FILL)
		{
			const int& cellsPerColumn = _rows;
			
			const float& spaceOccupiedByCells = cellsPerColumn * _cellSize.Y;

			const float& finalAvailableVerticalSpace = availableVerticalSpace - spaceOccupiedByCells;

			if (finalAvailableVerticalSpace < 0)
			{
				AdjustCellsCountWithClamp(_rows, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellTopMargin, _cellBottomMargin});
				currentYPosition = minYBounds;
				return;
			}

			_cellTopMargin = finalAvailableVerticalSpace / cellsPerColumn / 2;
			_cellBottomMargin = _cellTopMargin;
			AdjustCellsCountWithClamp(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});
			currentYPosition = minYBounds + _cellTopMargin;
			return;
		}

		AdjustCellsCountWithClamp(_rows, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellTopMargin, _cellBottomMargin});		
		
		const int& cellsPerColumn = _rows;

		const float& spaceOccupiedPerCell = _cellTopMargin + _cellSize.Y + _cellBottomMargin;

		if (_gridVerticalAlignment == EGridVerticalAlignment::CENTER)
		{
			int halfCells = FMath::Floor(cellsPerColumn / 2);

			minYBounds += availableVerticalSpace / 2 - halfCells * spaceOccupiedPerCell;

			if ((cellsPerColumn &  1) == 1)
			{
				minXBounds -= spaceOccupiedPerCell / 2;
			}
		}

		if (_gridVerticalAlignment == EGridVerticalAlignment::BOTTOM)
		{
			minYBounds = maxYBounds - cellsPerColumn * (_cellTopMargin + _cellSize.Y + _cellBottomMargin);
		}

		currentYPosition = minYBounds + _cellTopMargin;

		return;
	}

	AdjustCellsCountWithClamp(_rows, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellTopMargin, _cellBottomMargin});
	
	const int& availableHorizontalSpace = maxXBounds - minXBounds;
		
	if (_gridHorizontalAlignment == EGridHorizontalAlignment::FILL)
	{
		const int& cellsPerRow = _columns;

		const float& spaceOccupiedByCells = cellsPerRow * _cellSize.X;

		const float& finalAvailableHorizontalSpace = availableHorizontalSpace - spaceOccupiedByCells;  

		if (finalAvailableHorizontalSpace < 0)
		{
			AdjustCellsCountWithClamp(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});			
			currentXPosition = minXBounds;
			return;
		}
		
		_cellLeftMargin = finalAvailableHorizontalSpace / cellsPerRow / 2;
		_cellRightMargin = _cellLeftMargin;		
		currentXPosition = minXBounds + _cellLeftMargin;
		return;
	}

	AdjustCellsCountWithClamp(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});
		
	const int& cellsPerRow = _columns;

	const float& spaceOccupiedPerCell = _cellLeftMargin + _cellSize.X + _cellRightMargin;

	if (_gridHorizontalAlignment == EGridHorizontalAlignment::CENTER)
	{
		int halfCells = FMath::Floor(cellsPerRow / 2);

		minXBounds += availableHorizontalSpace / 2 - halfCells * spaceOccupiedPerCell;

		if ((cellsPerRow &  1) == 1)
		{
			minXBounds -= spaceOccupiedPerCell / 2;
		}
	}

	if (_gridHorizontalAlignment == EGridHorizontalAlignment::RIGHT)
	{
		minXBounds = maxXBounds - cellsPerRow * spaceOccupiedPerCell;
	}

	currentXPosition = minXBounds + _cellLeftMargin;
}

void UGrid::AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds,
	const FVector2D& margins)
{
	int currentCheckPosition = minBounds;
	
	cellsPerLine = 0;
		
	while (currentCheckPosition + margins.X + cellSize + margins.Y <= maxBounds)
	{
		cellsPerLine++;
		currentCheckPosition += margins.X + cellSize + margins.Y;
	}
}

void UGrid::AdjustCellsCountWithClamp(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds,
	const FVector2D& margins)
{
	int currentCheckPosition = minBounds;
	
	const int currentCellsPerLine = cellsPerLine;
	
	cellsPerLine = 0;
	
	while (currentCheckPosition + margins.X + cellSize + margins.Y <= maxBounds && cellsPerLine < currentCellsPerLine)
	{
		cellsPerLine++;
		currentCheckPosition += margins.X + cellSize + margins.Y;
	}
}

void UGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition, const int& minYBounds, int& currentYPosition)
{
	for (int i = 0; i < _columns; ++i)
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

void UGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition, int& currentYPosition)
{		
	for (int i = 0; i < _rows; ++i)
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