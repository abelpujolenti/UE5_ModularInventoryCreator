// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularInventoryCreator.h"

#include "Editor/FGridStructureEditor.h"
#include "Editor/FScrollPropertiesEditor.h"
#include "Inventory/Grid/BaseGridStructure.h"

#define LOCTEXT_NAMESPACE "FModularInventoryCreatorModule"

void FModularInventoryCreatorModule::StartupModule()
{
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	propertyEditorModule.RegisterCustomClassLayout(
		UBaseGridStructure::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridStructureEditor::MakeInstance));

	propertyEditorModule.RegisterCustomClassLayout(
		UBaseScrollBarProperties::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FScrollPropertiesEditor::MakeInstance));
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModularInventoryCreatorModule::ShutdownModule()
{
	FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	propertyModule.UnregisterCustomClassLayout(UBaseGridStructure::StaticClass()->GetFName());
	propertyModule.UnregisterCustomClassLayout(UBaseScrollBarProperties::StaticClass()->GetFName());
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModularInventoryCreatorModule, ModularInventoryCreator)