// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Grid/ScrollableGrid.h"

#include "Blueprint/WidgetTree.h"
#include "Inventory/Cell.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Interfaces/IGridItemDataSource.h"
#include "Inventory/Scroll/BaseScrollBar.h"
#include "Observers/Observer.h"
#include "ModularInventoryCreator/Debug/DebugManager.h"

UScrollableGrid::~UScrollableGrid()
{
	_scrollObserver->UnsubscribeAll();
	delete _scrollObserver;
}

void UScrollableGrid::InitializeGrid(const UBaseGridStructure& gridStructure)
{	
	_extraLines = gridStructure.extraLines * 2;

	_scrollDistanceMultiplier = gridStructure.scrollDistanceMultiplier;

	_displacement = 0;
	
	Super::InitializeGrid(gridStructure);

	std::map<EScrollKeys, std::function<float(float)>> scrollObserverMap;

	scrollObserverMap.try_emplace(DISPLACEMENT, [&](float displacement)
	{
		_displacement = displacement;
		return _displacement;
	});

	scrollObserverMap.try_emplace(MAXIMUM_DISPLACEMENT, [&](float maximumDisplacement)
	{
		_maxScrollDisplacement = maximumDisplacement;
		return _maxScrollDisplacement;		
	});

	_scrollObserver = new ObserverMap<EScrollKeys, float>(scrollObserverMap);

	_scrollObserver->SetValue(MAXIMUM_DISPLACEMENT, _maxScrollDisplacement);		

	InitScroll();
}

ObserverMap<EScrollKeys, float>* const UScrollableGrid::GetScrollObserver() const
{
	return _scrollObserver;
}

void UScrollableGrid::InitScroll()
{
	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		_totalCells = (_columns + _extraLines) * _rows;
		
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

	_totalCells = (_rows + _extraLines) * _columns;

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

void UScrollableGrid::CalculateVerticalDisplacement() const
{
	int itemsCount {_gridItemDataSource->Execute_GetItemsCount(_gridItemDataSource->_getUObject())};

	int totalRows = itemsCount / _columns;

	if (itemsCount % _columns != 0)
	{
		totalRows++;
	}

	_scrollObserver->SetValue(MAXIMUM_DISPLACEMENT,
		-((_cellTopMargin + _cellSize.Y + _cellBottomMargin) * (totalRows - _rows)));
}

void UScrollableGrid::CalculateHorizontalDisplacement() const
{
	int itemsCount {_gridItemDataSource->Execute_GetItemsCount(_gridItemDataSource->_getUObject())};

	int totalColumns = itemsCount / _rows;

	if (itemsCount % _rows != 0)
	{
		totalColumns++;
	}

	_scrollObserver->SetValue(MAXIMUM_DISPLACEMENT,
		-((_cellLeftMargin + _cellSize.X + _cellRightMargin) * (totalColumns - _columns)));
}

void UScrollableGrid::InstantiateWidgets()
{
	Super::InstantiateWidgets();

	_clippingSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(_canvas->AddChildToCanvas(_clippingSizeBox))};
	FVector2D canvasPanelSlotPosition {canvasPanelSlot->GetPosition()};

	_clippingSizeBox->SetRenderTranslation({_gridPadding.Left, _gridPadding.Top});

	Cast<UCanvasPanelSlot>(_clippingSizeBox->Slot)
		->SetSize({-_gridPadding.Left + _gridDimensions.X - _gridPadding.Right,
			-_gridPadding.Top + _gridDimensions.Y - _gridPadding.Bottom});

	_clippingCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	_clippingSizeBox->AddChild(_clippingCanvas);

	//_clippingCanvas->SetClipping(EWidgetClipping::ClipToBounds);

	float halfExtraLines = _extraLines / 2;

	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		float cellHeight = _cellTopMargin + _cellSize.Y + _cellBottomMargin;
		_scrollBuffer = cellHeight * halfExtraLines;
		_minimumScrollPosition = canvasPanelSlotPosition.Y - _scrollBuffer;
		_maximumScrollPosition = canvasPanelSlotPosition.Y + canvasPanelSlot->GetSize().Y + _scrollBuffer;
		return;
	}

	float cellWidth = _cellLeftMargin + _cellSize.X + _cellRightMargin;
	_scrollBuffer = cellWidth * halfExtraLines;
	_minimumScrollPosition = canvasPanelSlotPosition.X - _scrollBuffer;
	_maximumScrollPosition = canvasPanelSlotPosition.X + canvasPanelSlot->GetSize().X + _scrollBuffer;
}

void UScrollableGrid::ResetScroll_Implementation()
{
	Execute_Scroll(this, -_displacement);
}

void UScrollableGrid::Scroll_Implementation(float deltaDistance)
{
	float lastScrollOffset = _displacement;

	int scrollDisplacement = deltaDistance * _scrollDistanceMultiplier;

	_displacement += scrollDisplacement;

	_displacement = FMath::Clamp(_displacement, _maxScrollDisplacement, 0);

	UDebugManager::DebugMessage(FString::Printf(TEXT("Max Scroll Displacement: %f\n"), _maxScrollDisplacement));
	UDebugManager::DebugMessage(FString::Printf(TEXT("Displacement: %f"), _displacement));

	_scrollFunction(_displacement - lastScrollOffset);

	_scrollObserver->SetValue(DISPLACEMENT, _displacement);
}

EGridOrientation UScrollableGrid::GetOrientation_Implementation() const
{
	return _gridOrientation;
}

float UScrollableGrid::GetWidth_Implementation() const
{
	return _gridDimensions.X;
}

float UScrollableGrid::GetHeight_Implementation() const
{
	return _gridDimensions.Y;
}

float UScrollableGrid::GetLength_Implementation() const
{
	return _getLengthFunction();
}

FVector2D UScrollableGrid::GetPivot_Implementation() const
{
	return _gridPivot;
}

float UScrollableGrid::GetMaximumDisplacement_Implementation() const
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
			TObjectPtr<UCell> cell {CreateWidget<UCell>(world, _gridItemDataSource->Execute_GetCellClass(_gridItemDataSource->_getUObject()))};
			cell->SetGridIndex(_rows * i + j);
			FillCell(cell, cell->GetGridIndex());
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(_clippingCanvas->AddChild(cell))};
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
			TObjectPtr<UCell> cell {CreateWidget<UCell>(world, _gridItemDataSource->Execute_GetCellClass(_gridItemDataSource->_getUObject()))};
			cell->SetGridIndex(_columns * i + j);
			FillCell(cell, cell->GetGridIndex());
			TObjectPtr<UCanvasPanelSlot> canvasPanelSlot {Cast<UCanvasPanelSlot>(_clippingCanvas->AddChildToCanvas(cell))};
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

FReply UScrollableGrid::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float wheelData {InMouseEvent.GetWheelDelta()};
	
	Execute_Scroll(this, wheelData);
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollableGrid::ScrollVertical(float deltaDistance)
{
	float extraBuffer {CalculateExtraBuffer()};
	
	for (int i = 0; i < _rows + _extraLines; ++i)
	{
		for (int j = 0; j < _columns; ++j)
		{
			TObjectPtr<UCell> cell {_cellGrid[i][j]};
			
			TObjectPtr<UCanvasPanelSlot> cellSlot {Cast<UCanvasPanelSlot>(cell->Slot)};

			FVector2D position {cellSlot->GetPosition()};

			position.Y = CalculateCellPosition(cell, deltaDistance, position.Y, extraBuffer);

			cellSlot->SetPosition(position);
		}
	}
}

void UScrollableGrid::ScrollHorizontal(float deltaDistance)
{
	float extraBuffer {CalculateExtraBuffer()};
	
	for (int i = 0; i < _columns + _extraLines; ++i)
	{
		for (int j = 0; j < _rows; ++j)
		{
			TObjectPtr<UCell> cell {_cellGrid[i][j]};
			
			TObjectPtr<UCanvasPanelSlot> cellSlot {Cast<UCanvasPanelSlot>(cell->Slot)};

			FVector2D position {cellSlot->GetPosition()};

			position.X = CalculateCellPosition(cell, deltaDistance, position.X, extraBuffer);

			cellSlot->SetPosition(position);
		}
	}
}

float UScrollableGrid::CalculateExtraBuffer() const
{
	if (_displacement > -_scrollBuffer)
	{
		return -_scrollBuffer - _displacement;
	}
	
	if (_displacement < _maxScrollDisplacement + _scrollBuffer)
	{
		return _maxScrollDisplacement + _scrollBuffer - _displacement;
	}

	return 0;
}

float UScrollableGrid::CalculateCellPosition(TObjectPtr<UCell> cell, float deltaDistance, float currentPosition, float extraBuffer) const
{	
	if (_displacement == _minScrollDisplacement && deltaDistance <= 0 ||
		_displacement == _maxScrollDisplacement && deltaDistance >= 0)
	{
		return currentPosition;
	}
	
	float nextPosition {currentPosition + deltaDistance};

	float minimumLimitPosition = _minimumScrollPosition;
	float maximumLimitPosition = _maximumScrollPosition;

	minimumLimitPosition -= extraBuffer;
	maximumLimitPosition -= extraBuffer;
	
	if (nextPosition >= minimumLimitPosition && nextPosition <= maximumLimitPosition)
	{
		return nextPosition;
	}	

	if (nextPosition < minimumLimitPosition)
	{
		cell->SetGridIndex(cell->GetGridIndex() + _totalCells);		
		FillCell(cell, cell->GetGridIndex());
		return maximumLimitPosition - (minimumLimitPosition - nextPosition);
	}

	cell->SetGridIndex(cell->GetGridIndex() - _totalCells);
	FillCell(cell, cell->GetGridIndex());
	return minimumLimitPosition + std::fmod(nextPosition, maximumLimitPosition);

	/*if (nextPosition < minimumLimitPosition)
	{
		cell->SetGridIndex(cell->GetGridIndex() + _totalCells);
	}
	else
	{
		cell->SetGridIndex(cell->GetGridIndex() - _totalCells);
	}

	FillCell(cell, cell->GetGridIndex());

	float rangeWrapped = std::fmod(nextPosition - minimumLimitPosition, maximumLimitPosition - minimumLimitPosition);

	if(rangeWrapped < 0)
	{
		rangeWrapped += maximumLimitPosition - minimumLimitPosition;
	}
	
	return rangeWrapped + minimumLimitPosition;*/
}