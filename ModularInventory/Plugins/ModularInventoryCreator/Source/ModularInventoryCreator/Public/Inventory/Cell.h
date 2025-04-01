// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "Components/Button.h"
#include "Cell.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Abstract)
class MODULARINVENTORYCREATOR_API UCell : public UInventoryWidgetBase
{
	
public:

	UFUNCTION(BlueprintCallable, Category = "Cell|Size")
	virtual void SetCellSize(FVector2D newSize);

	UFUNCTION(BlueprintCallable, Category = "Cell|Size")
	virtual FVector2D GetCellSize() const;

	UFUNCTION(BlueprintCallable, Category = "Cell")
	virtual void OnClick() PURE_VIRTUAL(UCell::OnClick);

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _backgroundCellImage = nullptr;

	FVector2D _cellSize;
	
private:
	
	GENERATED_BODY()
};
