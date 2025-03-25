// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "ScrollableGrid.generated.h"

/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UScrollableGrid : public UGrid
{

protected:

	virtual void NativeOnInitialized() override;

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, const int& maxXBounds,
		int& currentXPosition, const int& maxYBounds, int& currentYPosition) override;

	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& maxXBounds, int& currentXPosition,
		const int& minYBounds, const int& maxYBounds, int& currentYPosition) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	int _cellsExtraLines;

private:
	GENERATED_BODY()
};
