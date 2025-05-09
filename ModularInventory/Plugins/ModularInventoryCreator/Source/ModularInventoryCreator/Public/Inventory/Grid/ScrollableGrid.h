// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "../Private/Interfaces/IScrollable.h"
#include "ScrollableGrid.generated.h"

/**
 * 
 */
class UScrollBox;
class UScroll;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTest, float, number);

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UScrollableGrid : public UBaseGrid, public IIScrollable
{
	friend class UFactoryGrid;
	
public:
	virtual void Scroll(float deltaDistance) override;
	
	virtual EGridOrientation GetOrientation() const override;
	
	virtual float GetLength() const override;
	
	virtual float GetMaximumDisplacement() const override;

protected:

	virtual void InitializeGrid(const UGridStructure& gridStructure) override;

	void InitScroll();

	virtual void InstantiateGrid() override;

	virtual void SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource) override;
	void CalculateVerticalDisplacement();
	void CalculateHorizontalDisplacement();

	virtual void InstantiateWidgets() override;

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void ScrollVertical(float deltaDistance);
	virtual void ScrollHorizontal(float deltaDistance);

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world) override;	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world) override;

	UPROPERTY()
	TObjectPtr<UCanvasPanel> _clippingCanvas = nullptr;

	UPROPERTY()
	TObjectPtr<USizeBox> _clippingSizeBox = nullptr;
	
	UPROPERTY()
	int _extraLines;

	UPROPERTY()
	float _scrollDistanceMultiplier;

	UPROPERTY()
	float _minScrollDisplacement {0};

	UPROPERTY()
	float _maxScrollDisplacement;

	UPROPERTY()
	float _currentScrollOffset;

	std::function<void(float)> _scrollFunction;

	std::function<float()> _getLengthFunction;

	std::function<void()> _calculateDisplacementFunction;

private:	
	
	GENERATED_BODY()
};