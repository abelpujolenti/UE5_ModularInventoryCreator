// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "Cell.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UCell : public UInventoryWidgetBase
{
	
public:

	UFUNCTION(BlueprintCallable, Category = "Cell|Size")
	void SetCellSize(FVector2D newSize);

	UFUNCTION(BlueprintCallable, Category = "Cell|Size")
	FVector2D GetCellSize() const;

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _backgroundCellImage = nullptr;

	FVector2D _cellSize;
	
private:
	
	GENERATED_BODY()
};
