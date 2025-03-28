// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularInventoryCreator.h"

#include "Editor/FGridEditor.h"

#define LOCTEXT_NAMESPACE "FModularInventoryCreatorModule"

void FModularInventoryCreatorModule::StartupModule()
{
	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	propertyModule.RegisterCustomClassLayout(
		"Grid",
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridEditor::MakeInstance));
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModularInventoryCreatorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		propertyModule.UnregisterCustomClassLayout("Grid");
	}
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModularInventoryCreatorModule, ModularInventoryCreator)