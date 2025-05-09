// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FactoryInterfaceInstance.generated.h"

/**
 * 
 */
UCLASS()
class MODULARINVENTORYCREATOR_API UFactoryInterfaceInstance : public UGameInstanceSubsystem
{
public:
	template<typename TInterface, typename TBaseClass, typename = std::enable_if_t<std::is_base_of_v<TInterface, TBaseClass>>>
	static TScriptInterface<TInterface> CreateInterfaceInstance(TSubclassOf<TBaseClass> classObject)
	{
		check(classObject)

		TBaseClass* newObjectInstance = NewObject<TBaseClass>(GetTransientPackage(), classObject);

		check(newObjectInstance);

		TScriptInterface<TInterface> scriptInterface;
		scriptInterface.SetObject(newObjectInstance);
		scriptInterface.SetInterface(Cast<TInterface>(newObjectInstance));

		return scriptInterface;
	}	
	
private:
	GENERATED_BODY()
};
