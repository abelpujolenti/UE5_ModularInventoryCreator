// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/ScrollableGrid.h"

#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UScrollableGrid::NativeOnInitialized()
{
	_cellsExtraLines *= 2;
	
	Super::NativeOnInitialized();
}

void UScrollableGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
	int& currentYPosition)
{	
	Super::CreateHorizontalGrid(world, minXBounds, currentXPosition, currentYPosition);
	
	for (int i = 0; i < _cellsExtraLines; ++i)
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

void UScrollableGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition, const int& minYBounds,
	int& currentYPosition)
{	
	Super::CreateVerticalGrid(world, currentXPosition, minYBounds, currentYPosition);

	for (int i = 0; i < _cellsExtraLines; ++i)
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
