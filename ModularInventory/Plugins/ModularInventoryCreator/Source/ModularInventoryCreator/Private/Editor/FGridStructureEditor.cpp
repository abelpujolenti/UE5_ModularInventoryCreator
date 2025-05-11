#include "FGridStructureEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Grid/BaseGridStructure.h"
#include "Inventory/Grid/BaseGrid.h"

#define LOCTEXT_NAMESPACE "FGridStructureEditor"

TSharedRef<IDetailCustomization> FGridStructureEditor::MakeInstance()
{
	return MakeShareable(new FGridStructureEditor);
}

void FGridStructureEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> isScrollable = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, isScrollable));
	TSharedPtr<IPropertyHandle> extraLines = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, extraLines));
	TSharedPtr<IPropertyHandle> scrollDistanceMultiplier = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, scrollDistanceMultiplier));
	TSharedPtr<IPropertyHandle> hasScrollBar = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, hasScrollBar));
	TSharedPtr<IPropertyHandle> scrollBarClass = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, scrollBarClass));
	
	TSharedPtr<IPropertyHandle> useCellsToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, useCellsToShapeGrid));
	TSharedPtr<IPropertyHandle> gridDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, gridDimensions));
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, fillGridWithCells));

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, gridOrientation));
	TSharedPtr<IPropertyHandle> gridVerticalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, gridVerticalAlignment));
	TSharedPtr<IPropertyHandle> gridHorizontalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, gridHorizontalAlignment));
	
	TSharedPtr<IPropertyHandle> rows = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, rows));
	TSharedPtr<IPropertyHandle> columns = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, columns));

	TSharedPtr<IPropertyHandle> useCellClassSizes = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, useCellClassSizes));
	TSharedPtr<IPropertyHandle> cellSize = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, cellSize));	

	TSharedPtr<IPropertyHandle> cellTopMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, cellTopMargin));
	TSharedPtr<IPropertyHandle> cellLeftMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, cellLeftMargin));
	TSharedPtr<IPropertyHandle> cellRightMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, cellRightMargin));
	TSharedPtr<IPropertyHandle> cellBottomMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseGridStructure, cellBottomMargin));
	
	auto RefreshEditor = [&DetailBuilder]()
	{
		DetailBuilder.ForceRefreshDetails();
	};

	isScrollable->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	hasScrollBar->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	useCellsToShapeGrid->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridOrientation->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	fillGridWithCells->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	useCellClassSizes->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridHorizontalAlignment->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	gridVerticalAlignment->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));

	bool isItScrollable;
	bool itHasScrollBar;
	bool itUsesCellsToShapeGrid;
	bool itFillsGridWithCells;
	EGridOrientation orientation;
	bool itUsesCellClassSizes;
	
	isScrollable->GetValue(isItScrollable);
	hasScrollBar->GetValue(itHasScrollBar);
	useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
	fillGridWithCells->GetValue(itFillsGridWithCells);
	gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation));
	useCellClassSizes->GetValue(itUsesCellClassSizes);

	if (!isItScrollable)
	{
		DetailBuilder.HideProperty(extraLines);
		DetailBuilder.HideProperty(scrollDistanceMultiplier);
		DetailBuilder.HideProperty(hasScrollBar);
		DetailBuilder.HideProperty(scrollBarClass);
	}

	if (!itHasScrollBar)
	{
		DetailBuilder.HideProperty(scrollBarClass);
	}

	DetailBuilder.EditDefaultProperty(extraLines)
		->DisplayName(orientation == EGridOrientation::VERTICAL
			? FText::FromString("Extra Columns")
			: FText::FromString("Extra Rows"));	

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