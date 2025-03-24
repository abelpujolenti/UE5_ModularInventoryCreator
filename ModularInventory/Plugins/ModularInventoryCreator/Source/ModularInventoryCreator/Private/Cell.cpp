// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "CustomLog.h"

#include "Components/Image.h"

void UCell::SetCellSize(float newSize) const
{
	BackgroundCellImage->SetBrushSize(FVector2D(newSize, newSize));
}
