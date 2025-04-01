// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "../Private/Interfaces/IScrollable.h"
#include "ScrollableGrid.generated.h"

/**
 * 
 */
class UScroll;

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UScrollableGrid : public UGrid, public IIScrollable
{

	friend class FScrollableGridEditor;

public:
	virtual void Scroll(float deltaDistance) override;
	
	virtual EGridOrientation GetOrientation() const override;
	
	virtual float GetLength() const override;
	
	virtual float GetMaximumDisplacement() const override;

protected:

	virtual ~UScrollableGrid() override = default;
	
	UFUNCTION(BlueprintCallable, Category = "Grid|Content")
	virtual void SetGridDataSource(UTestDataSource* gridDataSource) override;

	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, int& currentXPosition,
		int& currentYPosition) override;

	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, int& currentXPosition,	const int& minYBounds,
		int& currentYPosition) override;

	UPROPERTY(EditInstanceOnly, Category = "Grid")
	bool _isScrollable;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Scrollable", meta = (ClampMin = 1, UIMin = 1))
	int _extraLines;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Scrollable")
	TObjectPtr<UScroll> _scroll = nullptr;

private:	
	
	GENERATED_BODY()
};