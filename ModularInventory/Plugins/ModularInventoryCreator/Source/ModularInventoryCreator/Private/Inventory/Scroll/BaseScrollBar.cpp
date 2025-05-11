// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Scroll/BaseScrollBar.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Observers/Observer.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "ModularInventoryCreator/Debug/DebugManager.h"

void UBaseScrollBar::InitializeScrollBar(const UBaseScrollBarProperties& scrollBarProperties,
	TScriptInterface<IIScrollable> scrollable)
{	
	_thickness = scrollBarProperties.thickness;
	_scrollBarTexture = scrollBarProperties.barTexture;
	_scrollTexture = scrollBarProperties.scrollTexture;
	_scrollable = scrollable;
	
	_gridOrientation = _scrollable->Execute_GetOrientation(_scrollable->_getUObject());

	if (!scrollBarProperties.snapToScrollable)
	{
		_relativePositionToScrollablePivot = scrollBarProperties.positionRelativeToScrollablePivot;
	}
	else
	{
		FVector2D pivot {_scrollable->Execute_GetPivot(_scrollable->_getUObject())};
		
		if (_gridOrientation == EGridOrientation::VERTICAL)
		{
			_relativePositionToScrollablePivot = {pivot.X, pivot.Y + _scrollable->Execute_GetHeight(_scrollable->_getUObject())};
		}
		else
		{
			_relativePositionToScrollablePivot = {pivot.X + _scrollable->Execute_GetWidth(_scrollable->_getUObject()), pivot.Y};
		}
	}
	
	if (!scrollBarProperties.adjustLengthToScrollable)
	{
		_length = scrollBarProperties.length;		
	}
	else
	{
		if (_gridOrientation == EGridOrientation::VERTICAL)
		{
			_length = _scrollable->Execute_GetHeight(_scrollable->_getUObject());
		}
		else
		{
			_length = _scrollable->Execute_GetWidth(_scrollable->_getUObject());
		}
	}

	InstantiateScrollBar();
}

void UBaseScrollBar::InstantiateScrollBar()
{
	InstantiateWidgets();

	SetMaximumDisplacement(_scrollable->Execute_GetMaximumDisplacement(_scrollable->_getUObject()));

	ObserverMap<EScrollKeys, float>* scrollObserver {_scrollable->GetScrollObserver()};

	_displacementSubscription = scrollObserver->Subscribe(DISPLACEMENT, [&](float displacement)
	{
		SetDisplacement(displacement);
	});

	_maximumDisplacementSubscription = scrollObserver->Subscribe(MAXIMUM_DISPLACEMENT, [&](float maximumDisplacement)
	{
		SetMaximumDisplacement(maximumDisplacement);
	});
}


void UBaseScrollBar::InstantiateWidgets()
{
	TObjectPtr<UCanvasPanel> rootCanvasPanel {WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass())};
	WidgetTree->RootWidget = rootCanvasPanel;

	_scrollBarSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	rootCanvasPanel->AddChildToCanvas(_scrollBarSizeBox);

	_scrollBarSizeBox->SetRenderTranslation(_relativePositionToScrollablePivot);
	
	_scrollBarImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	_scrollBarSizeBox->AddChild(_scrollBarImage);

	_scrollBarImage->SetBrushResourceObject(_scrollBarTexture);

	FVector2D barSize;

	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		barSize = {_length, _thickness};
	}
	else
	{
		barSize = {_thickness, _length};
	}

	Cast<UCanvasPanelSlot>(_scrollBarSizeBox->Slot)->SetSize(barSize);
	_scrollBarImage->SetBrushSize(barSize);	

	_scrollSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	rootCanvasPanel->AddChildToCanvas(_scrollSizeBox);

	_scrollSizeBox->SetRenderTranslation(_relativePositionToScrollablePivot);

	_scrollImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	_scrollSizeBox->AddChild(_scrollImage);

	_scrollImage->SetBrushResourceObject(_scrollTexture);
}

void UBaseScrollBar::SetDisplacement(float displacement)
{
	_displacement = displacement;
	
	UDebugManager::DebugMessage(FString::Printf(TEXT("Displacement: %f\n"), displacement));
}

void UBaseScrollBar::SetMaximumDisplacement(float maximumDisplacement)
{
	_maximumDisplacement = maximumDisplacement;
	UDebugManager::DebugMessage(FString::Printf(TEXT("Maximum Displacement: %f\n"), _maximumDisplacement));

	FVector2D scrollSize;

	if (_gridOrientation == EGridOrientation::VERTICAL)
	{
		scrollSize = {(_length / (_length - _maximumDisplacement)) * _length, _thickness};
	}
	else
	{
		scrollSize = {_thickness, (_length / (_length - _maximumDisplacement)) * _length};	
	}	
	
	Cast<UCanvasPanelSlot>(_scrollSizeBox->Slot)->SetSize(scrollSize);
	_scrollImage->SetBrushSize(scrollSize);
}