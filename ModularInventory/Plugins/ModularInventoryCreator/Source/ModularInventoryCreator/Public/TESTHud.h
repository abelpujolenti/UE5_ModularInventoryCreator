// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TESTHud.generated.h"

class UGrid;
class UInventoryWidgetBase;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API ATESTHud : public AHUD
{

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGrid> inventoryWidgetClass = nullptr;

private:

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UGrid> _inventoryWidget = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FVector2D _gridPivot;

	UPROPERTY(EditDefaultsOnly)
	FVector2D _gridDimensions;

	UPROPERTY(EditDefaultsOnly)
	float _gridHorizontalMargin;

	UPROPERTY(EditDefaultsOnly)
	float _gridVerticalMargin;

	UPROPERTY(EditDefaultsOnly)
	float _cellSize;

	UPROPERTY(EditDefaultsOnly)
	float _cellSpace;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1, UIMin = 1))
	int _extraLines;
	
	GENERATED_BODY()
};