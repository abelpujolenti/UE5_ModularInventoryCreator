// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Cell.h"

#include "Components/Image.h"

void UCell::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_cellSize = _backgroundCellImage->GetBrush().GetImageSize();
}

void UCell::SetCellSize(FVector2D newSize)
{
	_backgroundCellImage->SetBrushSize(newSize);
	_cellSize = newSize;
}

FVector2D UCell::GetCellSize() const
{
	return _cellSize;
}