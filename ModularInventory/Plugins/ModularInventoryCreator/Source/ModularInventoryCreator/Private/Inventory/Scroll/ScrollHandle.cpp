// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Scroll/ScrollHandle.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"


void UScrollHandle::InitScrollHandle(UTexture2D texture, bool isOrientationVertical, FVector2D position)
{
	_texture = texture;
	_position = position;

	if (isOrientationVertical)
	{
		_scrollFunction = [&](FVector2D mousePosition)
		{
			ScrollVertical(mousePosition);
		};
	}
	else
	{
		_scrollFunction = [&](FVector2D mousePosition)
		{
			ScrollHorizontal(mousePosition);
		};
	}
	
	InstantiateWidgets();
}

void UScrollHandle::InstantiateWidgets()
{	
	TObjectPtr<UCanvasPanel> rootCanvasPanel {WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass())};
	WidgetTree->RootWidget = rootCanvasPanel;

	_sizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	rootCanvasPanel->AddChildToCanvas(_sizeBox);

	_image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	_sizeBox->AddChild(_image);

	_image->SetBrushResourceObject(_texture);
}

FReply UScrollHandle::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		_isDragging = true;
		_dragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		//return FReply::Handled().CaptureMouse(TSharedRef<SWidget>(this));	
	}*/
	
	return FReply::Unhandled();
}

FReply UScrollHandle::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	_isDragging = false;
	return FReply::Handled().ReleaseMouseCapture();
}

FReply UScrollHandle::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!_isDragging)
	{
		return FReply::Unhandled();	
	}

	FVector2D mousePosition {InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition())};

	

	return FReply::Handled();	
}

void UScrollHandle::SetPosition(FVector2d position)
{
	_position = position;
}

void UScrollHandle::SetSize(FVector2D size)
{
	_image->SetBrushSize(size);
}

void UScrollHandle::ScrollVertical(FVector2D mousePosition)
{
	float newYPosition = mousePosition.Y - _dragOffset.Y;

	//float maximumYDisplacement =  
}

void UScrollHandle::ScrollHorizontal(FVector2D mousePosition)
{
	
}
