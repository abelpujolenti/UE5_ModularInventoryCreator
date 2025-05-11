// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryWidgetBase.h"
#include "ScrollHandle.generated.h"

class UImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UScrollHandle : public UInventoryWidgetBase
{
public:

	void InitScrollHandle(UTexture2D texture, bool isOrientationVertical, FVector2D position);

	virtual void InstantiateWidgets() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetPosition(FVector2d position);

	void SetSize(FVector2D size);
	
private:

	void ScrollVertical(FVector2D mousePosition);
	void ScrollHorizontal(FVector2D mousePosition);

	std::function<void(FVector2D)> _scrollFunction;

	TObjectPtr<UTexture2D> _texture;

	TObjectPtr<USizeBox> _sizeBox;

	TObjectPtr<UImage> _image;

	bool _isDragging;
	FVector2D _dragOffset;	
	FVector2D _position;
	
	GENERATED_BODY()
};
