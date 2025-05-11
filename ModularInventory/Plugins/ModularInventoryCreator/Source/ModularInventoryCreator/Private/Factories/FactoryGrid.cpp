// Fill out your copyright notice in the Description page of Project Settings.

#include "Factories/FactoryGrid.h"

#include "Components/CanvasPanel.h"
#include "Inventory/Scroll/BaseScrollBar.h"

UBaseGrid* UFactoryGrid::CreateGrid(UCanvasPanel* canvas, TSubclassOf<UBaseGridStructure> gridStructureClass)
{
	UBaseGridStructure* gridStructure {NewObject<UBaseGridStructure>(GetTransientPackage(), gridStructureClass)};

	checkf(gridStructure, TEXT("A BaseGridStructure class misses"));
	
	if (gridStructure->isScrollable)
	{
		return ReturnScrollableGrid(canvas, *gridStructure); 
	}

	return ReturnGrid(canvas, *gridStructure);
}

TObjectPtr<UGrid> UFactoryGrid::ReturnGrid(UCanvasPanel* canvas, const UBaseGridStructure& gridStructure) const
{	
	TObjectPtr<UWorld> world {GetWorld()};
	
	TObjectPtr<UGrid> grid {CreateWidget<UGrid>(world, UGrid::StaticClass())};

	grid->InitializeGrid(gridStructure);

	canvas->AddChildToCanvas(grid);

	return grid;
}

TObjectPtr<UScrollableGrid> UFactoryGrid::ReturnScrollableGrid(UCanvasPanel* canvas, const UBaseGridStructure& gridStructure) const
{
	TObjectPtr<UWorld> world {GetWorld()};
	
	TObjectPtr<UScrollableGrid> scrollableGrid {CreateWidget<UScrollableGrid>(world, UScrollableGrid::StaticClass())};

	scrollableGrid->InitializeGrid(gridStructure);

	if (gridStructure.hasScrollBar)
	{
		UBaseScrollBarProperties* scrollBarProperties {NewObject<UBaseScrollBarProperties>(GetTransientPackage(), gridStructure.scrollBarClass)};

		checkf(scrollBarProperties, TEXT("A BaseScrollBarProperties class misses"));
		
		TObjectPtr<UBaseScrollBar> scrollBar {CreateWidget<UBaseScrollBar>(scrollableGrid, UBaseScrollBar::StaticClass())};

		scrollBar->InitializeScrollBar(*scrollBarProperties, scrollableGrid);

		canvas->AddChildToCanvas(scrollBar);
	}

	canvas->AddChildToCanvas(scrollableGrid);

	return scrollableGrid;
}