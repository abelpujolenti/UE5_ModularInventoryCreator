// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugManager.generated.h"

/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UDebugManager : public UGameInstanceSubsystem
{
public:
	
	static void DebugMessage(FString message, int32 index = -1, float time = 1.f, FColor color = FColor::Cyan);
	
private:
	GENERATED_BODY()
};
