// Fill out your copyright notice in the Description page of Project Settings.

#include "Factories/FactoryGrid.h"

UBaseGrid* UFactoryGrid::CreateGrid(TSubclassOf<UGridStructure> gridStructureClass)
{
	UGridStructure* gridStructure = NewObject<UGridStructure>(GetTransientPackage(), gridStructureClass);

	check(gridStructure);
	
	if (gridStructure->isScrollable)
	{
		return ReturnScrollableGrid(*gridStructure); 
	}

	return ReturnGrid(*gridStructure);
}

TObjectPtr<UGrid> UFactoryGrid::ReturnGrid(const UGridStructure& gridStructure) const
{	
	TObjectPtr<UWorld> world {GetWorld()};
	
	TObjectPtr<UGrid> grid {CreateWidget<UGrid>(world, UGrid::StaticClass())};

	grid->InitializeGrid(gridStructure);

	return grid;
}

TObjectPtr<UScrollableGrid> UFactoryGrid::ReturnScrollableGrid(const UGridStructure& gridStructure) const
{
	TObjectPtr<UWorld> world {GetWorld()};
	
	TObjectPtr<UScrollableGrid> scrollableGrid {CreateWidget<UScrollableGrid>(world, UScrollableGrid::StaticClass())};

	scrollableGrid->InitializeGrid(gridStructure);

	return scrollableGrid;
}