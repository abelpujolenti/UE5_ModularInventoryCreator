// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Scroll/Scroll.h"

#include "Components/Image.h"
#include "Interfaces/IScrollable.h"

void UScroll::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBarImage->SetBrushSize(FVector2D{_width, _height});
}

void UScroll::SetScrollable(const TScriptInterface<IIScrollable>& scrollable)
{
	_scrollable = scrollable;

	_length = _scrollable->GetLength();

	_maximumDisplacement = _scrollable->GetMaximumDisplacement();

	_gridOrientation = _scrollable->GetOrientation();

	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		_scrollImage->SetBrushSize(FVector2D{_length / (_length + _maximumDisplacement), _height});
		return;
	}

	_scrollImage->SetBrushSize(FVector2D{_width, _length / (_length + _maximumDisplacement)});
}