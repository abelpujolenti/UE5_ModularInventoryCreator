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

	friend class FScrollableGridEditor;

public:

protected:

	virtual ~UScrollableGrid() override = default;

	virtual void NativeOnInitialized() override;

	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
		int& currentYPosition) override;

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition,	const int& minYBounds,
		int& currentYPosition) override;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Scrollable", meta = (ClampMin = 1, UIMin = 1))
	int _cellsExtraLines;

private:	
	
	GENERATED_BODY()
};
