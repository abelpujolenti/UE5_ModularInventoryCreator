// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
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

	UFUNCTION(BlueprintCallable, Category = "Cell")
	void SetGridIndex(int gridIndex);

	UFUNCTION(BlueprintCallable, Category = "Cell")
	int GetGridIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Cell")
	virtual void Clear() PURE_VIRTUAL(UCell::Clear);

	UFUNCTION(BlueprintCallable, Category = "Cell")
	void Hide();

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _backgroundCellImage = nullptr;

	FVector2D _cellSize;
	
private:

	int _gridIndex;
	
	GENERATED_BODY()
};
