// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularInventoryCreator.h"

#include "Editor/FGridEditor.h"
#include "Editor/FScrollableGridEditor.h"
#include "Inventory/Grid/Grid.h"
#include "Inventory/Grid/ScrollableGrid.h"
#include "Inventory/Scroll/Scroll.h"

#define LOCTEXT_NAMESPACE "FModularInventoryCreatorModule"

void FModularInventoryCreatorModule::StartupModule()
{
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	propertyEditorModule.RegisterCustomClassLayout(
		UGrid::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridEditor::MakeInstance));

	propertyEditorModule.RegisterCustomClassLayout(
		UScrollableGrid::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FScrollableGridEditor::MakeInstance));

	propertyEditorModule.RegisterCustomClassLayout(
		UScroll::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FScrollableGridEditor::MakeInstance));
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModularInventoryCreatorModule::ShutdownModule()
{
	FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	propertyModule.UnregisterCustomClassLayout(UGrid::StaticClass()->GetFName());
	propertyModule.UnregisterCustomClassLayout(UScrollableGrid::StaticClass()->GetFName());
	propertyModule.UnregisterCustomClassLayout(UScroll::StaticClass()->GetFName());
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModularInventoryCreatorModule, ModularInventoryCreator)