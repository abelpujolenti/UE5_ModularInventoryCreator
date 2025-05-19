// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Interfaces/IScrollable.h"
#include "ScrollableGrid.generated.h"

/**
 * 
 */
class UScrollBox;
class UBaseScrollBar;

template<typename TKey, typename TValue>
class ObserverMap;

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UScrollableGrid : public UBaseGrid, public IIScrollable
{
	friend class UFactoryGrid;
	
public:

	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual void ResetScroll_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual void Scroll_Implementation(float deltaDistance) override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual EGridOrientation GetOrientation_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual float GetWidth_Implementation() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual float GetHeight_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual float GetLength_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual FVector2D GetPivot_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Scroll")
	virtual float GetMaximumDisplacement_Implementation() const override;

	virtual ObserverMap<EScrollKeys, float>* const UScrollableGrid::GetScrollObserver() const override;

	virtual ~UScrollableGrid() override;

protected:

	virtual void InitializeGrid(const UBaseGridStructure& gridStructure) override;

	void InitScroll();

	virtual void InstantiateGrid() override;

	virtual void SetGridDataSource(TSubclassOf<UBaseItemDataSource> gridDataSource) override;
	void CalculateVerticalDisplacement() const;
	void CalculateHorizontalDisplacement() const;

	virtual void InstantiateWidgets() override;	

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world) override;	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world) override;
	
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void ScrollVertical(float deltaDistance);
	void ScrollHorizontal(float deltaDistance);

	float CalculateExtraBuffer() const;
	float CalculateCellPosition(TObjectPtr<UCell> cell, float deltaDistance, float currentPosition, float extraBuffer) const;

	UPROPERTY()
	TObjectPtr<UCanvasPanel> _clippingCanvas = nullptr;

	UPROPERTY()
	TObjectPtr<USizeBox> _clippingSizeBox = nullptr;
	
	int _extraLines;

	int _totalCells;

	float _scrollDistanceMultiplier;
	float _displacement;
	float _minScrollDisplacement {0};
	float _maxScrollDisplacement;

	float _scrollBuffer;
	float _minimumScrollPosition;
	float _maximumScrollPosition;

	std::function<void(float)> _scrollFunction;

	std::function<float()> _getLengthFunction;

	std::function<void()> _calculateDisplacementFunction;

	ObserverMap<EScrollKeys, float>* _scrollObserver = nullptr;

private:	
	
	GENERATED_BODY()
};
