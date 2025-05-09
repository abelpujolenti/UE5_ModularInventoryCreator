// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/ScrollDetector.h"

FReply UScrollDetector::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float mouseDelta = InMouseEvent.GetWheelDelta();

	OnScroll(mouseDelta > 0);

	return FReply::Handled();
}
