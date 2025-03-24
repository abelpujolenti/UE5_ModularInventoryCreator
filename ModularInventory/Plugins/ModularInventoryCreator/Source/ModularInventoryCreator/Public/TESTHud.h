// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TESTHud.generated.h"

class UInventoryWidgetBase;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API ATESTHud : public AHUD
{

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidgetBase> inventoryWidgetClass = nullptr;

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	TObjectPtr<UWorld> _world = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetBase> _inventoryWidget = nullptr;

	UPROPERTY(EditInstanceOnly)
	float _size;
	
	GENERATED_BODY()
};