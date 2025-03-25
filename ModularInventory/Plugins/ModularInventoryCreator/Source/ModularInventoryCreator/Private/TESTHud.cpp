// Fill out your copyright notice in the Description page of Project Settings.


#include "TESTHud.h"

#include "InventoryWidgetBase.h"

void ATESTHud::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UWorld> world = GetWorld();

	checkf(world, TEXT("Unable to get a reference of the world"));

	checkf(inventoryWidgetClass, TEXT("Invalid InventoryWidgetClass"));
	
	_inventoryWidget = CreateWidget<UUserWidget>(world, inventoryWidgetClass);
	_inventoryWidget->AddToViewport();
	_inventoryWidget->SetVisibility(ESlateVisibility::Visible);
}
