// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/ScrollableGrid.h"

#include "Blueprint/WidgetTree.h"
#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Interfaces/IGridItemDataSource.h"
#include "Inventory/Scroll/Scroll.h"
#include "ModularInventoryCreator/Debug/DebugManager.h"

void UScrollableGrid::InitializeGrid(const UGridStructure& gridStructure)
{	
	_extraLines = gridStructure.extraLines * 2;

	_scrollDistanceMultiplier = gridStructure.scrollDistanceMultiplier;

	_currentScrollOffset = 0;
	
	Super::InitializeGrid(gridStructure);

	InitScroll();
}

void UScrollableGrid::InitScroll()
{
	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		_scrollFunction = [&](float deltaDistance)
		{
			ScrollHorizontal(deltaDistance);
		};

		_getLengthFunction = [&]()
		{
			return _gridDimensions.Y;	
		};

		_calculateDisplacementFunction = [&]()
		{
			CalculateHorizontalDisplacement();
		};

		_calculateDisplacementFunction();
		
		return;
	}

	_scrollFunction = [&](float deltaDistance)
	{
		ScrollVertical(deltaDistance);
	};
	
	_getLengthFunction = [&]()
	{
		return _gridDimensions.X;	
	};

	_calculateDisplacementFunction = [&]()
	{
		CalculateVerticalDisplacement();
	};

	_calculateDisplacementFunction();
}

void UScrollableGrid::InstantiateGrid()
{	
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

void UScrollableGrid::SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource)
{
	Super::SetGridDataSource(gridDataSource);

	_calculateDisplacementFunction();
}

void UScrollableGrid::CalculateVerticalDisplacement()
{
	int itemsCount {_gridDataSource->Execute_GetItemsCount(_gridDataSource->_getUObject())};

	int totalRows = itemsCount / _columns;

	if (itemsCount % _columns != 0)
	{
		totalRows++;
	}

	_maxScrollDisplacement = -((_cellTopMargin + _cellSize.Y + _cellBottomMargin) * (totalRows - _rows));	
}

void UScrollableGrid::CalculateHorizontalDisplacement()
{
	int itemsCount {_gridDataSource->Execute_GetItemsCount(_gridDataSource->_getUObject())};

	int totalColumns = itemsCount / _rows;

	if (itemsCount % _rows != 0)
	{
		totalColumns++;
	}

	_maxScrollDisplacement = (_cellLeftMargin + _cellSize.X + _cellRightMargin) * (totalColumns - _columns); 
}

void UScrollableGrid::InstantiateWidgets()
{
	Super::InstantiateWidgets();

	_clippingSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	_canvas->AddChildToCanvas(_clippingSizeBox);

	_clippingSizeBox->SetRenderTranslation({_gridPadding.Left, _gridPadding.Top});

	Cast<UCanvasPanelSlot>(_clippingSizeBox->Slot)
		->SetSize({-_gridPadding.Left + _gridDimensions.X - _gridPadding.Right,
			-_gridPadding.Top + _gridDimensions.Y - _gridPadding.Bottom});

	_clippingCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	_clippingSizeBox->AddChild(_clippingCanvas);

	//_clippingCanvas->SetClipping(EWidgetClipping::ClipToBounds);
}

FReply UScrollableGrid::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float wheelData {InMouseEvent.GetWheelDelta()};
	
	Scroll(wheelData);
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollableGrid::Scroll(float deltaDistance)
{
	float lastScrollOffset = _currentScrollOffset;

	int scrollDisplacement = deltaDistance * _scrollDistanceMultiplier;

	_currentScrollOffset += scrollDisplacement;

	_currentScrollOffset = FMath::Clamp(_currentScrollOffset, _maxScrollDisplacement, 0);

	_scrollFunction(_currentScrollOffset - lastScrollOffset);
	
	if (GEngine)
	{
		UDebugManager::DebugMessage(FString::Printf(TEXT("DeltaDistance: %f\n"), deltaDistance));
		UDebugManager::DebugMessage(FString::Printf(TEXT("LastScrollOffset: %f"), lastScrollOffset));
		UDebugManager::DebugMessage(FString::Printf(TEXT("CurrentScrollOffset: %f"), _currentScrollOffset));
		UDebugManager::DebugMessage(FString::Printf(TEXT("MaxDisplacement: %f"), _maxScrollDisplacement));
	}
}

void UScrollableGrid::ScrollVertical(float deltaDistance)
{
	for (int i = 0; i < _rows + _extraLines; ++i)
	{
		for (int j = 0; j < _columns; ++j)
		{
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(_cellGrid[i][j]->Slot)};

			FVector2D position {canvasPanelSlot->GetPosition()};

			canvasPanelSlot->SetPosition(FVector2D(position.X, position.Y + deltaDistance));
		}
	}	
	
}

void UScrollableGrid::ScrollHorizontal(float deltaDistance)
{
	for (int i = 0; i < _columns + _extraLines; ++i)
	{
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(_cellGrid[i][j]->Slot)};

			FVector2D position {canvasPanelSlot->GetPosition()};

			canvasPanelSlot->SetPosition(FVector2D(position.X + deltaDistance, position.Y));
		}
	}	
}

EGridOrientation UScrollableGrid::GetOrientation() const
{
	return _gridOrientation;
}

float UScrollableGrid::GetLength() const
{
	return _getLengthFunction();
}

float UScrollableGrid::GetMaximumDisplacement() const
{
	return _maxScrollDisplacement;
}

void UScrollableGrid::CreateVerticalGrid(const TObjectPtr<UWorld>& world)
{
	int currentXPosition = _cellLeftMargin;
	int currentYPosition = _cellTopMargin;
	
	for (int i = 0; i < _columns + _extraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));
			FillCell(cell, _rows * i + j);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_clippingCanvas->AddChild(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);
			newLine.Add(cell);
			currentYPosition += _cellTopMargin + _cellSize.Y + _cellBottomMargin;
		}
		_cellGrid.Add(newLine);
		currentYPosition = _cellTopMargin;
		currentXPosition += _cellLeftMargin + _cellSize.X + _cellRightMargin;
	}
}

void UScrollableGrid::CreateHorizontalGrid(const TObjectPtr<UWorld>& world)
{
	int currentXPosition = _cellLeftMargin;
	int currentYPosition = _cellTopMargin;
	
	for (int i = 0; i < _rows + _extraLines; ++i)
	{
		TArray<TObjectPtr<UCell>> newLine;
		for (int j = 0; j < _columns; ++j)
		{
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridDataSource->Execute_GetCellClass(_gridDataSource->_getUObject()));			
			FillCell(cell, _columns * i + j);
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot = Cast<UCanvasPanelSlot>(_clippingCanvas->AddChildToCanvas(cell));
			canvasPanelSlot->SetPosition(FVector2D(currentXPosition, currentYPosition));
			canvasPanelSlot->SetSize(_cellSize);	
			newLine.Add(cell);
			currentXPosition += _cellLeftMargin + _cellSize.X + _cellRightMargin;
		}
		_cellGrid.Add(newLine);
		currentXPosition = _cellLeftMargin;
		currentYPosition += _cellTopMargin + _cellSize.Y + _cellBottomMargin;
	}
}