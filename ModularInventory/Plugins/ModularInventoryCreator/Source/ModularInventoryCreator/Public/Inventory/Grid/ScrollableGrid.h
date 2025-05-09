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

	virtual void InstantiateGrid() override;

	virtual void InstantiateWidgets() override;

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition,	const int& minYBounds,
		int& currentYPosition) override;
	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
		int& currentYPosition) override;

	UPROPERTY()
	TObjectPtr<UScrollBox> _scrollBox = nullptr;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> _scrollBoxSlot = nullptr;
	
	UPROPERTY()
	int _extraLines;
	
	UPROPERTY()
	TObjectPtr<UScroll> _scroll = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Test")
	FTest _test;

	UFUNCTION(BlueprintCallable, Category = "Test")
	void CallMe(float currentOffset);

private:	
	
	GENERATED_BODY()
};