#include "Inventory/Grid/Grid.h"

#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Interfaces/IGridItemDataSource.h"

void UGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world)
{
	int minXBounds = _gridPadding.Left;
	int currentXPosition = minXBounds + _cellLeftMargin;

	int minYBounds = _gridPadding.Top;
	int currentYPosition = minYBounds + _cellTopMargin;
	
	for (int i = 0; i < _columns; ++i)
	{		
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));
			FillCell(cell, _rows * i + j);
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

void UGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world)
{
	int minXBounds = _gridPadding.Left;
	int currentXPosition = minXBounds + _cellLeftMargin;

	int minYBounds = _gridPadding.Top;
	int currentYPosition = minYBounds + _cellTopMargin;
	
	for (int i = 0; i < _rows; ++i)
	{		
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _columns; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));
			FillCell(cell, _columns * i + j);
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
