// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Grid/BaseGrid.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Inventory/Cell.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Factories/FactoryInterfaceInstance.h"
#include "Interfaces/IGridItemDataSource.h"

void UBaseGrid::InitializeGrid(const UGridStructure& gridStructure)
{	
	_gridPivot = gridStructure.gridPivot;
	_useCellsToShapeGrid = gridStructure.useCellsToShapeGrid;
	_gridDimensions = gridStructure.gridDimensions;
	_gridPadding = gridStructure.gridPadding;
	_gridOrientation = gridStructure.gridOrientation;
	_fillGridWithCells = gridStructure.fillGridWithCells;
	_gridVerticalAlignment = gridStructure.gridVerticalAlignment;
	_gridHorizontalAlignment = gridStructure.gridHorizontalAlignment;
	_background = gridStructure.background;
	_gridDataSourceClass = gridStructure.gridDataSourceClass;
	_useCellClassSizes = gridStructure.useCellClassSizes;
	_columns = gridStructure.columns;
	_rows = gridStructure.rows;
	_cellSize = gridStructure.cellSize;
	_cellTopMargin = gridStructure.cellTopMargin;
	_cellLeftMargin = gridStructure.cellLeftMargin;
	_cellRightMargin = gridStructure.cellRightMargin;
	_cellBottomMargin = gridStructure.cellBottomMargin;

	InstantiateGrid();
}

void UBaseGrid::InstantiateGrid()
{	
	InitGridDataSource(_gridDataSourceClass);

	TObjectPtr<UWorld> world = {GetWorld()};

	checkf(world, TEXT("Unable to get a reference of the world"));
	if (_useCellClassSizes)
	{
		TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));
		_cellSize = cell->GetCellSize();
	}

	if (_useCellsToShapeGrid)
	{
		_gridDimensions = FVector2D(_gridPadding.Left + _columns * (_cellLeftMargin + _cellSize.X + _cellRightMargin) + _gridPadding.Right,
			_gridPadding.Top + _rows * (_cellTopMargin + _cellSize.Y + _cellBottomMargin) + _gridPadding.Bottom);
	}
	
	InstantiateWidgets();

	CreateBoundaries(world);
}

void UBaseGrid::SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSourceClass)
{
	InitGridDataSource(gridDataSourceClass);

	_fillFunction();
}

void UBaseGrid::InitGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSourceClass)
{
	_gridDataSource = UFactoryInterfaceInstance::CreateInterfaceInstance<IIGridItemDataSource, UBaseItemDataSource>(gridDataSourceClass);
}

void UBaseGrid::InstantiateWidgets()
{
	TObjectPtr<UCanvasPanel> rootCanvasPanel {WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass())};
	WidgetTree->RootWidget = rootCanvasPanel;

	_sizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	rootCanvasPanel->AddChildToCanvas(_sizeBox);
		
	_sizeBox->SetRenderTranslation(_gridPivot);

	_sizeBoxSlot = Cast<UCanvasPanelSlot>(_sizeBox->Slot);
	_sizeBoxSlot->SetSize(_gridDimensions);

	_canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	_sizeBox->AddChild(_canvas);

	TObjectPtr<UImage> background {WidgetTree->ConstructWidget<UImage>(UImage::StaticClass())};

	FSlateBrush brush;

	brush.SetResourceObject(_background);
	brush.SetImageSize(_gridDimensions);
	
	background->SetBrush(brush);

	_canvas->AddChildToCanvas(background);

	TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(background->Slot)};
	
	canvasPanelSlot->SetAutoSize(true);
}

void UBaseGrid::CreateBoundaries(const TObjectPtr<UWorld>& world)
{
	int minXBounds = _gridPadding.Left;
	int maxXBounds = _gridDimensions.X - _gridPadding.Right;
	int initialXPosition = minXBounds + _cellLeftMargin;

	int minYBounds = _gridPadding.Top;
	int maxYBounds = _gridDimensions.Y - _gridPadding.Bottom;
	int initialYPosition = minYBounds + _cellTopMargin;

	const bool isOrientationVertical = _gridOrientation == EGridOrientation::VERTICAL;

	if (!_useCellsToShapeGrid)
	{
		if (_fillGridWithCells)
		{	
			AdjustCellsCount(_columns, _cellSize.X, minXBounds, maxXBounds, {_cellLeftMargin, _cellRightMargin});
			AdjustCellsCount(_rows, _cellSize.Y, minYBounds, maxYBounds, {_cellTopMargin, _cellBottomMargin});
		}
		else
		{
			AdjustBoundaries(isOrientationVertical, minXBounds, maxXBounds, initialXPosition, minYBounds, maxYBounds, initialYPosition);
		}
	}
	
	if (isOrientationVertical)
	{
		CreateVerticalGrid(world);
		
		_fillFunction = [&]()
		{
			for (int i = 0; i < _columns; ++i)
			{
				for (int j = 0; j < _rows; ++j)
				{
					FillCell(_cellGrid[i][j], _rows * i + j);
				}
			}
		};
		return;
	}
	
	CreateHorizontalGrid(world);

	_fillFunction = [&]()
	{	
		for (int i = 0; i < _rows; ++i)
		{
			for (int j = 0; j < _columns; ++j)
			{
				FillCell(_cellGrid[i][j], _columns * i + j);
			}
		}
	};
}

void UBaseGrid::AdjustBoundaries(const bool isOrientationVertical, int& minXBounds, const int& maxXBounds, int& initialXPosition,
	int& minYBounds, const int& maxYBounds, int& initialYPosition)
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
				initialYPosition = minYBounds;
				return;
			}

			_cellTopMargin = finalAvailableVerticalSpace / cellsPerColumn / 2;
			_cellBottomMargin = _cellTopMargin;
			AdjustCellsCountWithClamp(_columns, _cellSize.X, minXBounds, maxXBounds, FVector2D{_cellLeftMargin, _cellRightMargin});
			initialYPosition = minYBounds + _cellTopMargin;
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

		initialYPosition = minYBounds + _cellTopMargin;

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
			initialXPosition = minXBounds;
			return;
		}
		
		_cellLeftMargin = finalAvailableHorizontalSpace / cellsPerRow / 2;
		_cellRightMargin = _cellLeftMargin;		
		initialXPosition = minXBounds + _cellLeftMargin;
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

	initialXPosition = minXBounds + _cellLeftMargin;
}

void UBaseGrid::AdjustCellsCount(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds,
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

void UBaseGrid::AdjustCellsCountWithClamp(int& cellsPerLine, const float& cellSize, const int& minBounds, const int& maxBounds,
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

void UBaseGrid::FillCell(TObjectPtr<UCell> cell, int index) const
{
	_gridDataSource->Execute_FillCellIndex(_gridDataSource->_getUObject(), cell, index);
}


#if WITH_EDITOR

void UBaseGrid::OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs)
{
	Super::OnDesignerChanged(EventArgs);
}

void UBaseGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
