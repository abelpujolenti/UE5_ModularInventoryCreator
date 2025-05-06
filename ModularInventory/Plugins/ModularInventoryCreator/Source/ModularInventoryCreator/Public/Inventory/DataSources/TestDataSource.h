// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IGridDataSource.h"
#include "TestDataSource.generated.h"

UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UTestDataSource : public UObject, public IIGridDataSource
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
