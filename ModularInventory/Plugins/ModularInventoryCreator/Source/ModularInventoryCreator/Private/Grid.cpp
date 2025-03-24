// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "CustomLog.h"

#include "Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"

void UGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sizeBoxCanvas = Cast<UCanvasPanelSlot>(_canvasSizeBox->Slot);

	_canvasSizeBoxSlot = Cast<USizeBoxSlot>(_canvas->Slot);	
}

void UGrid::SetGrid(TObjectPtr<UWorld> world, FVector2D pivot, FVector2D gridDimensions, float gridHorizontalPadding,
	float gridVerticalPadding, float cellSize, float cellSpace, int extraLines)
{		
	if (!_sizeBoxCanvas || !_canvasSizeBox || !_canvas || !_canvasSizeBoxSlot)
	{
		return;		
	}
	
	_canvasSizeBox->SetRenderTranslation(pivot);

	_sizeBoxCanvas->SetSize(gridDimensions);

	_canvasSizeBoxSlot->Padding.Left = gridHorizontalPadding;
	_canvasSizeBoxSlot->Padding.Right = gridHorizontalPadding;

	_canvasSizeBoxSlot->Padding.Top = gridVerticalPadding;
	_canvasSizeBoxSlot->Padding.Bottom = gridVerticalPadding;

	CreateCells(world, gridDimensions, gridHorizontalPadding, gridVerticalPadding, cellSize, cellSpace, extraLines);	
}

void UGrid::CreateCells(TObjectPtr<UWorld> world, FVector2D gridDimensions, float gridHorizontalPadding,
	float gridVerticalPadding, float cellSize, float cellSpace, int extraLines)
{
	int minXBounds = gridHorizontalPadding;
	int maxXBounds = gridDimensions.X - gridHorizontalPadding;
	int currentXPosition = minXBounds + cellSpace;

	int minYBounds = gridVerticalPadding;
	int maxYBounds = gridDimensions.Y - gridVerticalPadding;
	int currentYPosition = minYBounds + cellSpace;
	
	while (currentYPosition < maxYBounds)
	{		
		TArray<TObjectPtr<UCell>> newRow;
		while (currentXPosition + cellSize + cellSpace < maxXBounds)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(FVector2D(cellSize, cellSize));
			newRow.Add(cell);
			currentXPosition += cellSize + cellSpace * 2;
		}
		_cellGrid.Add(newRow);
		currentXPosition = minXBounds + cellSpace;
		currentYPosition += cellSize + cellSpace;
	}

	for (int i = 0; i < extraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newRow;
		while (currentXPosition + cellSize + cellSpace < maxXBounds)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, cellClass);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(FVector2D(cellSize, cellSize));	
			newRow.Add(cell);
			currentXPosition += cellSize + cellSpace * 2;		
		}
		_cellGrid.Add(newRow);
		currentXPosition = minXBounds + cellSpace;
		currentYPosition += cellSize + cellSpace;
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