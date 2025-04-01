// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryWidgetBase.h"
#include "Inventory/Grid/Grid.h"
#include "Scroll.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EVerticalOrientationPosition : uint8
{
	TOP UMETA(DisplayName = "Top"),
	BOTTOM UMETA(DisplayName = "Bottom")
};

UENUM(BlueprintType)
enum class EHorizontalOrientationPosition : uint8
{
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right")
};

UCLASS()
class MODULARINVENTORYCREATOR_API UScroll : public UInventoryWidgetBase
{

	friend class FScrollEditor;

public:

	UFUNCTION(BlueprintCallable, Category = "Scroll")
	void SetScrollable(UPARAM(ref)const TScriptInterface<IIScrollable>& scrollable);

	virtual ~UScroll() override = default;

protected:

	virtual void NativeOnInitialized() override;

	TScriptInterface<IIScrollable> _scrollable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Scroll")
	EVerticalOrientationPosition _verticalOrientationPosition;

	UPROPERTY(EditAnywhere, Category = "Scroll")
	EHorizontalOrientationPosition _horizontalOrientationPosition;

	UPROPERTY(EditInstanceOnly)
	float _width;

	UPROPERTY(EditInstanceOnly)
	float _height;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> _scrollBarImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> _scrollImage;

	float _length;

	float _currentDisplacement;

	float _maximumDisplacement;

	EGridOrientation _gridOrientation;
	
	GENERATED_BODY()
};
