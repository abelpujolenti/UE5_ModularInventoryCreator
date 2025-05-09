#include "FGridStructureEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/GridStructure.h"
#include "Inventory/Grid/BaseGrid.h"

#define LOCTEXT_NAMESPACE "FGridStructureEditor"

TSharedRef<IDetailCustomization> FGridStructureEditor::MakeInstance()
{
	return MakeShareable(new FGridStructureEditor);
}

void FGridStructureEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{	
	TSharedPtr<IPropertyHandle> useCellsToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, useCellsToShapeGrid));
	TSharedPtr<IPropertyHandle> gridDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, gridDimensions));
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, fillGridWithCells));

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, gridOrientation));
	TSharedPtr<IPropertyHandle> gridVerticalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, gridVerticalAlignment));
	TSharedPtr<IPropertyHandle> gridHorizontalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, gridHorizontalAlignment));
	
	TSharedPtr<IPropertyHandle> rows = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, rows));
	TSharedPtr<IPropertyHandle> columns = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, columns));

	TSharedPtr<IPropertyHandle> useCellClassSizes = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, useCellClassSizes));
	TSharedPtr<IPropertyHandle> cellSize = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, cellSize));	

	TSharedPtr<IPropertyHandle> cellTopMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, cellTopMargin));
	TSharedPtr<IPropertyHandle> cellLeftMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, cellLeftMargin));
	TSharedPtr<IPropertyHandle> cellRightMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, cellRightMargin));
	TSharedPtr<IPropertyHandle> cellBottomMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, cellBottomMargin));

	TSharedPtr<IPropertyHandle> isScrollable = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, isScrollable));
	TSharedPtr<IPropertyHandle> extraLines = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGridStructure, extraLines));
	
	auto RefreshEditor = [&DetailBuilder]()
	{
		DetailBuilder.ForceRefreshDetails();
	};

	useCellsToShapeGrid->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridOrientation->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	fillGridWithCells->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	useCellClassSizes->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridHorizontalAlignment->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridVerticalAlignment->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	isScrollable->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));

	bool itUsesCellsToShapeGrid;
	bool itFillsGridWithCells;
	EGridOrientation orientation;
	bool itUsesCellClassSizes;
	bool isItScrollable;
	
	useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
	fillGridWithCells->GetValue(itFillsGridWithCells);
	gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation));
	useCellClassSizes->GetValue(itUsesCellClassSizes);
	isScrollable->GetValue(isItScrollable);

	if (!isItScrollable)
	{
		DetailBuilder.HideProperty(extraLines);
	}

	if (itUsesCellsToShapeGrid || itFillsGridWithCells)
	{		
		DetailBuilder.HideProperty(gridVerticalAlignment);
		DetailBuilder.HideProperty(gridHorizontalAlignment);
	}

	if (orientation == EGridOrientation::VERTICAL)
	{
		DetailBuilder.HideProperty(gridHorizontalAlignment);
	}
	else
	{
		DetailBuilder.HideProperty(gridVerticalAlignment);		
	}

	if (itUsesCellClassSizes)
	{		
		DetailBuilder.HideProperty(cellSize);
	}

	FText displayName;
	
	if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
	{
		displayName = FText::FromString("");
	}
	else
	{
		displayName = orientation == EGridOrientation::VERTICAL ? FText::FromString("Extra Columns") : FText::FromString("Extra Rows");	
	}

	DetailBuilder.EditDefaultProperty(extraLines)->DisplayName(displayName);	

	if (itUsesCellsToShapeGrid)
	{		
		DetailBuilder.HideProperty(gridDimensions);
		DetailBuilder.HideProperty(fillGridWithCells);
		return;
	}
	
	if (itFillsGridWithCells)
	{
		DetailBuilder.HideProperty(columns);
		DetailBuilder.HideProperty(rows);
		return;
	}
	
	if (orientation == EGridOrientation::VERTICAL)
	{
		EGridVerticalAlignment verticalAlignment;
		gridVerticalAlignment->GetValue(reinterpret_cast<uint8&>(verticalAlignment));

		if (isItScrollable)
		{
			DetailBuilder.HideProperty(columns);			
		}

		if (verticalAlignment == EGridVerticalAlignment::FILL)
		{
			DetailBuilder.HideProperty(cellTopMargin);
			DetailBuilder.HideProperty(cellBottomMargin);
		}
		return;
	}

	EGridHorizontalAlignment horizontalAlignment;
	gridHorizontalAlignment->GetValue(reinterpret_cast<uint8&>(horizontalAlignment));

	if (isItScrollable)
	{
		DetailBuilder.HideProperty(rows);			
	}

	if (horizontalAlignment == EGridHorizontalAlignment::FILL)
	{
		DetailBuilder.HideProperty(cellLeftMargin);
		DetailBuilder.HideProperty(cellRightMargin);
	}	
}

#undef LOCTEXT_NAMESPACE