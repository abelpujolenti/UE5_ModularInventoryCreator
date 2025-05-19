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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetItemsCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UCell> GetCellClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FillCellIndex(UCell* cell, int index);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HasToHideCellIfEmpty(bool hasToHideCellIfEmpty);

private:
	GENERATED_BODY()	
};
