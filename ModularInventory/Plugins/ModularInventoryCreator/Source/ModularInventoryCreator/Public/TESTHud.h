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
	TSubclassOf<UUserWidget> inventoryWidgetClass = nullptr;

private:

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UUserWidget> _inventoryWidget = nullptr;
	
	GENERATED_BODY()
};