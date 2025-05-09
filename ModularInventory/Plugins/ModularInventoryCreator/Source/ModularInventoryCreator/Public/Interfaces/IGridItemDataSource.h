// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Inventory/Cell.h"

#include "IGridItemDataSource.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UIGridItemDataSource : public UInterface
{
	GENERATED_BODY()
};

class MODULARINVENTORYCREATOR_API IIGridItemDataSource
{
public:

	UFUNCTION(BlueprintNativeEvent)
	int GetItemsCount();

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UCell> GetCellClass();

	UFUNCTION(BlueprintNativeEvent)
	void FillCellIndex(UCell* cell, int index);

private:
	GENERATED_BODY()	
};
