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
	
	Execute_Scroll(this, wheelData);
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollableGrid::Scroll_Implementation(float deltaDistance)
{
	float lastScrollOffset = _displacement;

	int scrollDisplacement = deltaDistance * _scrollDistanceMultiplier;

	_displacement += scrollDisplacement;

	_displacement = FMath::Clamp(_displacement, _maxScrollDisplacement, 0);

	_scrollFunction(_displacement - lastScrollOffset);

	_scrollObserver->SetValue(DISPLACEMENT, _displacement);
	
	/*UDebugManager::DebugMessage(FString::Printf(TEXT("DeltaDistance: %f\n"), deltaDistance));
	UDebugManager::DebugMessage(FString::Printf(TEXT("LastScrollOffset: %f"), lastScrollOffset));
	UDebugManager::DebugMessage(FString::Printf(TEXT("CurrentScrollOffset: %f"), _displacement));
	UDebugManager::DebugMessage(FString::Printf(TEXT("MaxDisplacement: %f"), _maxScrollDisplacement));*/
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
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridItemDataSource->Execute_GetCellClass(_gridItemDataSource->_getUObject()));
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
			TObjectPtr<UCell> cell = CreateWidget<UCell>(world, _gridItemDataSource->Execute_GetCellClass(_gridItemDataSource->_getUObject()));			
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