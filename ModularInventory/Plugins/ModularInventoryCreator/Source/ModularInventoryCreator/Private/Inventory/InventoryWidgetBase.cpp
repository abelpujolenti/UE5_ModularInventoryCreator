// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidgetBase.h"

#if WITH_EDITOR
const FText UInventoryWidgetBase::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "CustomPaletteCategory", "Modular Inventory Creator");	
}
#endif
