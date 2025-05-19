// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DataSources/BaseItemDataSource.h"

#include "Test/TESTCell.h"

int UBaseItemDataSource::GetItemsCount_Implementation()
{
	return _items.Num();
}

TSubclassOf<UCell> UBaseItemDataSource::GetCellClass_Implementation()
{
	return _cellClass;
}

void UBaseItemDataSource::FillCellIndex_Implementation(UCell* cell, int index)
{
	UTESTCell* myCell = Cast<UTESTCell>(cell);

	if (index < _items.Num())
	{
		myCell->SetText(_items[index]);
		return;
	}

	if (_hasToHideCellIfEmpty)
	{
		myCell->Hide();
		return;
	}

	myCell->Clear();
}

void UBaseItemDataSource::HasToHideCellIfEmpty_Implementation(bool hasToHideCellIfEmpty)
{
	_hasToHideCellIfEmpty = hasToHideCellIfEmpty;
}
