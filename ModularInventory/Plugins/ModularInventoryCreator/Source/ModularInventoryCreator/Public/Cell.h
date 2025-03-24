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

	void SetCellSize(float newSize) const;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> BackgroundCellImage = nullptr;
	
private:
	
	GENERATED_BODY()
};
