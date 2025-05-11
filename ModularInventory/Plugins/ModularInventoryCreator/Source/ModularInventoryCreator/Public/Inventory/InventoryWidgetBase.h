// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

class UCanvasPanel;
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

protected:
	virtual void InstantiateWidgets() PURE_VIRTUAL(UInventoryWidgetBase::InstantiateWidgets);
	
private:	
	GENERATED_BODY()
};
