// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MODULARINVENTORYCREATOR_API UInventoryWidgetBase : public UUserWidget
{

public:
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	virtual void UpdateCellSize(float size) PURE_VIRTUAL(UInventoryWidgetBase::UpdateCellSize);
	
private:	
	GENERATED_BODY()
};
