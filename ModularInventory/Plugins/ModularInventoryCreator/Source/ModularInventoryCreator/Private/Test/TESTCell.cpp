// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TESTCell.h"

#include "Components/TextBlock.h"

void UTESTCell::OnClick()
{
	_textBlock->SetText(_text);
}

void UTESTCell::SetText(FText text)
{
	SetVisibility(ESlateVisibility::Visible);
	_textBlock->SetText(text);
}

void UTESTCell::Clear()
{
	SetVisibility(ESlateVisibility::Visible);
	_textBlock->SetText(FText::FromString(""));
}
