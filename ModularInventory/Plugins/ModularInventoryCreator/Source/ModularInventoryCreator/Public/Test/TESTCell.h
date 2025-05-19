// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Cell.h"
#include "TESTCell.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UTESTCell : public UCell
{
public:

	virtual void OnClick() override;

	void SetText(FText text);

	virtual void Clear() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> _textBlock = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Cell|Content")
	FText _text = FText::FromString("Default");
	
	GENERATED_BODY()
};
