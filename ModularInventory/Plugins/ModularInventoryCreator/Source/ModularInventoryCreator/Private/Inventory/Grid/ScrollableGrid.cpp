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

void UScrollableGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, const int& maxXBounds,
    int& currentXPosition, const int& maxYBounds, int& currentYPosition)
{	
	Super::CreateVerticalGrid(world, minXBounds, maxXBounds, currentXPosition, maxYBounds, currentYPosition);
	
	for (int i = 0; i < _cellsExtraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		while (currentXPosition + _cellSize.X + _cellsSpace <= maxXBounds)
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
		currentYPosition += _cellSize.Y + _cellsSpace;
	}
}

void UScrollableGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& maxXBounds,
	int& currentXPosition, const int& minYBounds, const int& maxYBounds, int& currentYPosition)
{	
	Super::CreateHorizontalGrid(world, maxXBounds, currentXPosition, minYBounds, maxYBounds, currentYPosition);

	for (int i = 0; i < _cellsExtraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		while (currentYPosition + _cellSize.Y + _cellsSpace <= maxYBounds)
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
		currentXPosition += _cellSize.X + _cellsSpace;
	}
}
