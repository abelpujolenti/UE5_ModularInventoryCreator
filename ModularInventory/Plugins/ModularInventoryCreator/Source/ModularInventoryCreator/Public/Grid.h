// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "Grid.generated.h"

class UCanvasPanelSlot;
class USizeBoxSlot;
class USizeBox;
class UCanvasPanel;
class UGridPanel;
class UCell;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UGrid : public UInventoryWidgetBase
{

public:

	void SetGrid(TObjectPtr<UWorld> world, FVector2D pivot, FVector2D gridDimensions, float gridHorizontalPadding,
		float gridVerticalPadding, float cellSize, float cellSpace, int extraLines);

	UPROPERTY(EditDefaultsOnly, Category = "Content|Cell")
	TSubclassOf<UCell> cellClass = nullptr;
	
#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void NativeOnInitialized() override;

	void CreateCells(TObjectPtr<UWorld> world, FVector2D gridDimensions, float gridHorizontalPadding,
		float gridVerticalPadding, float cellSize, float cellSpace, int extraLines);

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> _sizeBoxCanvas = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> _canvasSizeBox = nullptr;

	UPROPERTY()
	TObjectPtr<USizeBoxSlot> _canvasSizeBoxSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> _canvas = nullptr;

	TArray<TArray<TObjectPtr<UCell>>> _cellGrid;

private:
	
	GENERATED_BODY()
};
