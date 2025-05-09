// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/ScrollableGrid.h"

#include "Blueprint/WidgetTree.h"
#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Interfaces/IGridItemDataSource.h"
#include "Inventory/Scroll/Scroll.h"
#include "ModularInventoryCreator/Debug/DebugManager.h"

void UScrollableGrid::InitializeGrid(const UGridStructure& gridStructure)
{	
	_extraLines = gridStructure.extraLines;
	
	Super::InitializeGrid(gridStructure);
}

void UScrollableGrid::InstantiateGrid()
{	
	_extraLines *= 2;

	if (!_useCellsToShapeGrid && !_fillGridWithCells)
	{
		if (_gridOrientation == EGridOrientation::VERTICAL)
		{
			AdjustCellsCount(_columns, _cellSize.X, _gridPadding.Left,
				_gridDimensions.X - _gridPadding.Right,
				FVector2D{_cellLeftMargin, _cellRightMargin});
		}
		else
		{
			AdjustCellsCount(_rows, _cellSize.Y, _gridPadding.Top,
				_gridDimensions.Y - _gridPadding.Bottom,
				FVector2D{_cellTopMargin, _cellBottomMargin});
		}
	}

	Super::InstantiateGrid();
}

void UScrollableGrid::InstantiateWidgets()
{
	Super::InstantiateWidgets();
	
	_scrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass());

	_scrollBoxSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(_scrollBox));

	_scrollBoxSlot->SetSize(_gridDimensions);
}

FReply UScrollableGrid::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float wheelData {InMouseEvent.GetWheelDelta()};

	if (GEngine)
	{
		UDebugManager::DebugMessage(FString::Printf(TEXT("WheelData: %f"), wheelData));
	}
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollableGrid::Scroll(float deltaDistance)
{
}

EGridOrientation UScrollableGrid::GetOrientation() const
{
	return _gridOrientation;
}

float UScrollableGrid::GetLength() const
{
	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		return _gridDimensions.Y;
	}

	return _gridDimensions.X;
}

float UScrollableGrid::GetMaximumDisplacement() const
{
	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		return 1;
	}
	return 1;
}

void UScrollableGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition, const int& minYBounds,
	int& currentYPosition)
{	
	for (int i = 0; i < _columns + _extraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));
			FillCell(cell, _rows * i + j);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_sizeBox->AddChild(cell));
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

void UScrollableGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
    int& currentYPosition)
{		
	for (int i = 0; i < _rows + _extraLines; ++i)
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

void UScrollableGrid::CallMe(float currentOffset)
{
	_test.Broadcast(currentOffset);
}