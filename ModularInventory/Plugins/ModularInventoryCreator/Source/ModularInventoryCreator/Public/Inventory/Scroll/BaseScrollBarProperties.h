// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseScrollBarProperties.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARINVENTORYCREATOR_API UBaseScrollBarProperties : public UObject
{

public:

	UPROPERTY(EditAnywhere, Category = "Scrollbar")
	bool snapToScrollable;
	
	UPROPERTY(EditAnywhere, Category = "ScrollBar")
	FVector2D positionRelativeToScrollablePivot;

	UPROPERTY(EditAnywhere, Category = "Scrollbar")
	bool adjustLengthToScrollable;

	UPROPERTY(EditAnywhere, Category = "ScrollBar")
	float length;

	UPROPERTY(EditAnywhere, Category = "ScrollBar")
	float thickness;

	UPROPERTY(EditAnywhere, Category = "ScrollBar")
	TObjectPtr<UTexture2D> barTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "ScrollBar")
	TObjectPtr<UTexture2D> scrollTexture = nullptr;

private:
	GENERATED_BODY()
};
