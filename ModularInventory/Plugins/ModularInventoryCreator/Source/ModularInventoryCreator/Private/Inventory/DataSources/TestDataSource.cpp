// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DataSources/TestDataSource.h"

#include "Test/TESTCell.h"

int UTestDataSource::GetItemsCount_Implementation()
{
	return _items.Num();
}

TSubclassOf<UCell> UTestDataSource::GetCellClass_Implementation()
{
	return _cellClass;
}

void UTestDataSource::FillCellIndex_Implementation(UCell* cell, int index)
{
	UTESTCell* myCell = Cast<UTESTCell>(cell);
}
