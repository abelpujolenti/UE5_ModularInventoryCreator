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
	
	virtual void UpdateCellSize(float size) override;	

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Constituent Controls", meta = (BindWidget))
	TObjectPtr<UImage> BackgroundCellImage = nullptr;
	
private:
	
	GENERATED_BODY()
};
