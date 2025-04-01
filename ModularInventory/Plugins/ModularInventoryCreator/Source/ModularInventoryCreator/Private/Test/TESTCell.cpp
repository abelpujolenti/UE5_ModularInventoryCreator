// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TESTCell.h"

#include "Components/TextBlock.h"

void UTESTCell::OnClick()
{
	_textBlock->SetText(_text);
}
