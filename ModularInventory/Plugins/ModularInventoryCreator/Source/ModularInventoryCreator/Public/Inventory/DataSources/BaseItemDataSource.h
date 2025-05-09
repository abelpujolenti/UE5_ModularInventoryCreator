// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IGridItemDataSource.h"
#include "BaseItemDataSource.generated.h"

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UBaseItemDataSource : public UObject, public IIGridItemDataSource
{

public:

	UPROPERTY(EditAnywhere, Category = "Grid|Cell")
	TSubclassOf<UCell> _cellClass = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	virtual int GetItemsCount_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Cell")
	virtual TSubclassOf<UCell> GetCellClass_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Grid|Cell")
	virtual void FillCellIndex_Implementation(UCell* cell, int index) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid|Content")
	TArray<FText> _items;

private:
	
	GENERATED_BODY()
};
