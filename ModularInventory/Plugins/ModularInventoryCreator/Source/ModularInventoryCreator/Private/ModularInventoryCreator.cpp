// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularInventoryCreator.h"

#include "Editor/FGridStructureEditor.h"
#include "Inventory/GridStructure.h"

#define LOCTEXT_NAMESPACE "FModularInventoryCreatorModule"

void FModularInventoryCreatorModule::StartupModule()
{
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	propertyEditorModule.RegisterCustomClassLayout(
		UGridStructure::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridStructureEditor::MakeInstance));	
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModularInventoryCreatorModule::ShutdownModule()
{
	FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	propertyModule.UnregisterCustomClassLayout(UGridStructure::StaticClass()->GetFName());
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModularInventoryCreatorModule, ModularInventoryCreator)