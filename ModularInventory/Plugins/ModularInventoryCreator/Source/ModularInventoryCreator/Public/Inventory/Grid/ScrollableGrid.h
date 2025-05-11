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

	virtual void Scroll_Implementation(float deltaDistance) override;
	
	virtual EGridOrientation GetOrientation_Implementation() const override;

	virtual float GetWidth_Implementation() const override;
	virtual float GetHeight_Implementation() const override;
	virtual float GetLength_Implementation() const override;

	virtual FVector2D GetPivot_Implementation() const override;
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

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void ScrollVertical(float deltaDistance);
	virtual void ScrollHorizontal(float deltaDistance);

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world) override;	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world) override;

	UPROPERTY()
	TObjectPtr<UCanvasPanel> _clippingCanvas = nullptr;

	UPROPERTY()
	TObjectPtr<USizeBox> _clippingSizeBox = nullptr;
	
	int _extraLines;

	float _scrollDistanceMultiplier;
	float _displacement;
	float _minScrollDisplacement {0};
	float _maxScrollDisplacement;

	std::function<void(float)> _scrollFunction;

	std::function<float()> _getLengthFunction;

	std::function<void()> _calculateDisplacementFunction;

	ObserverMap<EScrollKeys, float>* _scrollObserver = nullptr;

private:	
	
	GENERATED_BODY()
};