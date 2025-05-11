// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Grid/BaseGridStructure.h"
#include "Inventory/Grid/ScrollableGrid.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FactoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UFactoryGrid : public UGameInstanceSubsystem
{

public:

	UFUNCTION(BlueprintCallable, Category = "Factory|Grid")
	UBaseGrid* CreateGrid(UCanvasPanel* canvas, TSubclassOf<UBaseGridStructure> gridStructure);

private:

	TObjectPtr<UGrid> ReturnGrid(UCanvasPanel* canvas, const UBaseGridStructure& gridStructure) const;
	TObjectPtr<UScrollableGrid> ReturnScrollableGrid(UCanvasPanel* canvas, const UBaseGridStructure& gridStructure) const;
	
	GENERATED_BODY()
};
