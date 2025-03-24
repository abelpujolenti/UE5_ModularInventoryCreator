// Fill out your copyright notice in the Description page of Project Settings.


#include "TESTHud.h"

#include "InventoryWidgetBase.h"

void ATESTHud::BeginPlay()
{
	Super::BeginPlay();

	_world = GetWorld();

	checkf(_world, TEXT("Unable to get a reference of the world"));

	checkf(inventoryWidgetClass, TEXT("Invalid InventoryWidgetClass"));

	_inventoryWidget = CreateWidget<UInventoryWidgetBase>(_world, inventoryWidgetClass);
	_inventoryWidget->AddToViewport();
	_inventoryWidget->SetVisibility(ESlateVisibility::Visible);
	_inventoryWidget->UpdateCellSize(_size);
}

void ATESTHud::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	_inventoryWidget->UpdateCellSize(_size);
}
