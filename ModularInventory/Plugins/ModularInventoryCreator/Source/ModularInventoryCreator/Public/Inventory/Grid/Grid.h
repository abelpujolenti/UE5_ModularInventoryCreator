// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Grid/BaseGrid.h"
#include <functional>

#include "Grid.generated.h"

class UIGridItemDataSource;
class UBaseItemDataSource;
class ATestDataSource;
class IIGridItemDataSource;
class UCanvasPanelSlot;
class USizeBoxSlot;
class USizeBox;
class UCanvasPanel;
class UGridPanel;
class UCell;
/**
 * 
 */

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UGrid : public UBaseGrid
{	
public:

#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	
	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition,	const int& minYBounds, int& currentYPosition) override;
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition, int& currentYPosition) override;	

private:
	
	GENERATED_BODY()
};