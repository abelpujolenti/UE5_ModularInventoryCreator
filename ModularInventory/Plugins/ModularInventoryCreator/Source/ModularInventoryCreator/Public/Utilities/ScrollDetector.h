// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollDetector.generated.h"

/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UScrollDetector : public UUserWidget
{

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Scroll")
	void OnScroll(bool scrollUp);

protected:

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	GENERATED_BODY()
};
