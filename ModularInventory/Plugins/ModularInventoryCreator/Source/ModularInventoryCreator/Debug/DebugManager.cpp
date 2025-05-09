// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugManager.h"

void UDebugManager::DebugMessage(FString message, int32 index, float time, FColor color)
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(index, time, color, message);
}
