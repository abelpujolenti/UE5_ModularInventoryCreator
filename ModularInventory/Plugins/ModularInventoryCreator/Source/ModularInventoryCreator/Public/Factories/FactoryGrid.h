// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/GridStructure.h"
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
	UBaseGrid* CreateGrid(TSubclassOf<UGridStructure> gridStructure);

private:

	TObjectPtr<UGrid> ReturnGrid(const UGridStructure& gridStructure) const;
	TObjectPtr<UScrollableGrid> ReturnScrollableGrid(const UGridStructure& gridStructure) const;
	
	GENERATED_BODY()
};
