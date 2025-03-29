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
		_gridDimensions = FVector2D(_gridPadding.Left + _cellsCount.row * (_cellMargins.Left + _cellSize.X + _cellMargins.Right) + _gridPadding.Right,
			_gridPadding.Top + _cellsCount.column * (_cellMargins.Top + _cellSize.Y + _cellMargins.Bottom) + _gridPadding.Bottom);
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
	int currentXPosition = minXBounds + _cellMargins.Left;

	int minYBounds = _gridPadding.Top;
	int maxYBounds = _gridDimensions.Y - _gridPadding.Bottom;
	int currentYPosition = minYBounds + _cellMargins.Top;

	const bool isOrientationVertical = _gridOrientation == EGridOrientation::VERTICAL;

	if (!_useCellsToShapeGrid && !_fillGridWithCells)
	{
		AdjustBoundaries(isOrientationVertical, minXBounds, maxXBounds, currentXPosition, minYBounds, maxYBounds, currentYPosition);
	}
	
	AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellMargins.Left, _cellMargins.Right});
	AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellMargins.Top, _cellMargins.Bottom});
	
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
		AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellMargins.Left, _cellMargins.Right});
		
		const int& availableVerticalSpace = maxYBounds - minYBounds;	

		if (_gridVerticalAlignment == EGridVerticalAlignment::FILL)
		{
			const int& cellsPerColumn = _cellsCount.column;
			
			const float& spaceOccupiedByCells = cellsPerColumn * _cellSize.Y;

			const float& finalAvailableVerticalSpace = availableVerticalSpace - spaceOccupiedByCells;

			if (finalAvailableVerticalSpace < 0)
			{
				_cellMargins.Top = 0;
				_cellMargins.Bottom = 0;
				AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellMargins.Top, _cellMargins.Bottom});
				currentYPosition = minYBounds;
				return;
			}

			_cellMargins.Top = finalAvailableVerticalSpace / cellsPerColumn / 2;
			_cellMargins.Bottom = _cellMargins.Top;
			AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellMargins.Left, _cellMargins.Right});
			currentYPosition = minYBounds + _cellMargins.Top;
			return;
		}

		AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellMargins.Top, _cellMargins.Bottom});		
		
		const int& cellsPerColumn = _cellsCount.column;

		const float& spaceOccupiedPerCell = _cellMargins.Top + _cellSize.Y + _cellMargins.Bottom;

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
			minYBounds = maxYBounds - cellsPerColumn * (_cellMargins.Top + _cellSize.Y + _cellMargins.Bottom);
		}

		currentYPosition = minYBounds + _cellMargins.Top;

		return;
	}

	AdjustCellsCount(_cellsCount.column, _cellSize.Y, minYBounds, maxYBounds, FVector2D{_cellMargins.Top, _cellMargins.Bottom});
	
	const int& availableHorizontalSpace = maxXBounds - minXBounds;
		
	if (_gridHorizontalAlignment == EGridHorizontalAlignment::FILL)
	{
		const int& cellsPerRow = _cellsCount.row;

		const float& spaceOccupiedByCells = cellsPerRow * _cellSize.X;

		const float& finalAvailableHorizontalSpace = availableHorizontalSpace - spaceOccupiedByCells;  

		if (finalAvailableHorizontalSpace < 0)
		{
			_cellMargins.Left = 0;
			_cellMargins.Right = 0;
			AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellMargins.Left, _cellMargins.Right});			
			currentXPosition = minXBounds;
			return;
		}
		
		_cellMargins.Left = finalAvailableHorizontalSpace / cellsPerRow / 2;
		_cellMargins.Right = _cellMargins.Left;		
		currentXPosition = minXBounds + _cellMargins.Left;
		return;
	}

	AdjustCellsCount(_cellsCount.row, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellMargins.Left, _cellMargins.Right});
		
	const int& cellsPerRow = _cellsCount.row;

	const float& spaceOccupiedPerCell = _cellMargins.Left + _cellSize.X + _cellMargins.Right;

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

	currentXPosition = minXBounds + _cellMargins.Left;
}

void UGrid::AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds,
	const FVector2D& margins) const
{
	int currentCheckPosition = minBounds;

	if (_fillGridWithCells)
	{		
		cellsPerLine = 0;
		
		while (currentCheckPosition + margins.X + cellSize + margins.Y <= maxBounds)
		{
			cellsPerLine++;
			currentCheckPosition += margins.X + cellSize + margins.Y;
		}
		return;
	}
	
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
			currentYPosition += _cellMargins.Top + _cellSize.Y + _cellMargins.Bottom;
		}
		_cellGrid.Add(newLine);
		currentYPosition = minYBounds + _cellMargins.Top;
		currentXPosition += _cellMargins.Left + _cellSize.X + _cellMargins.Right;
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
			currentXPosition += _cellMargins.Left + _cellSize.X + _cellMargins.Right;
		}
		_cellGrid.Add(newLine);
		currentXPosition = minXBounds + _cellMargins.Left;
		currentYPosition += _cellMargins.Top + _cellSize.Y + _cellMargins.Bottom;
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