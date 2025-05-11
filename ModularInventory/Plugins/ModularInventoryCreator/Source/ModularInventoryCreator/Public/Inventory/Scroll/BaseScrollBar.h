// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IScrollable.h"
#include "Inventory/InventoryWidgetBase.h"
#include "Inventory/Grid/Grid.h"
#include "BaseScrollBar.generated.h"

class UScrollableGrid;

template<typename T>
class ObserverSingleValue;

UCLASS()
class MODULARINVENTORYCREATOR_API UBaseScrollBar : public UInventoryWidgetBase
{
	friend class FScrollEditor;

public:

	void InitializeScrollBar(const UBaseScrollBarProperties& scrollBarProperties,
		TScriptInterface<IIScrollable> scrollable);

	virtual ~UBaseScrollBar() override = default;

protected:

	void InstantiateScrollBar();

	virtual void InstantiateWidgets() override;

	void SetDisplacement(float displacement);

	void SetMaximumDisplacement(float maximumDisplacement);

	TScriptInterface<IIScrollable> _scrollable = nullptr;

private:

	float _thickness;
	float _length;

	TObjectPtr<UTexture2D> _scrollBarTexture;
	TObjectPtr<UTexture2D> _scrollTexture;

	TObjectPtr<USizeBox> _scrollBarSizeBox;
	TObjectPtr<UImage> _scrollBarImage;

	TObjectPtr<USizeBox> _scrollSizeBox;
	TObjectPtr<UImage> _scrollImage;

	FVector2D _relativePositionToScrollablePivot;
	
	EGridOrientation _gridOrientation;

	float _displacement;
	float _maximumDisplacement;
	
	std::weak_ptr<std::function<void(float)>> _displacementSubscription;
	std::weak_ptr<std::function<void(float)>> _maximumDisplacementSubscription;
	
	GENERATED_BODY()
};