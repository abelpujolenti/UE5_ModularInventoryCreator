// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "CustomLog.h"

#include "Cell.h"
#include "DetailLayoutBuilder.h"
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
	const int& minXBounds = _gridPadding.Left;
	const int& maxXBounds = _gridDimensions.X - _gridPadding.Right;
	int currentXPosition = minXBounds + _cellsSpace;

	const int& minYBounds = _gridPadding.Top;
	const int& maxYBounds = _gridDimensions.Y - _gridPadding.Bottom;
	int currentYPosition = minYBounds + _cellsSpace;
	
	if (_gridOrientation == GridOrientation::VERTICAL)
	{
		CreateVerticalGrid(world, minXBounds, maxXBounds, currentXPosition, maxYBounds, currentYPosition);
		return;
	}

	CreateHorizontalGrid(world, maxXBounds, currentXPosition, minYBounds, maxYBounds, currentYPosition);
}

void UGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, const int& maxXBounds,
	int& currentXPosition, const int& maxYBounds, int& currentYPosition)
{	
	while (currentYPosition <= maxYBounds)
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
		currentYPosition += _cellSize.Y + _cellsSpace * 2;
	}
}

void UGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& maxXBounds, int& currentXPosition,
	const int& minYBounds, const int& maxYBounds, int& currentYPosition)
{
	while (currentXPosition <= maxXBounds)
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
		currentXPosition += _cellSize.X + _cellsSpace * 2;
	}
}

void UGrid::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> useCellToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellsToShapeGrid));
	TSharedPtr<IPropertyHandle> gridDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridDimensions));

	/*DetailBuilder.AddPropertyToCategory(gridDimensions).
		Visibility(_useCellsToShapeGrid ? EVisibility::Collapsed : EVisibility::Visible);*/

	DetailBuilder.AddPropertyToCategory(gridDimensions).
		Visibility(TAttribute<EVisibility>::Create([useCellToShapeGrid]()
		{
			bool enable;
			useCellToShapeGrid->GetValue(enable);
			return enable ? EVisibility::Collapsed : EVisibility::Visible;
		}));	
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