#include "FGridEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Grid/Grid.h"

#define LOCTEXT_NAMESPACE "FGridEditor"

TSharedRef<IDetailCustomization> FGridEditor::MakeInstance()
{
	return MakeShareable(new FGridEditor);
}

void FGridEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{	
	TSharedPtr<IPropertyHandle> useCellsToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellsToShapeGrid));
	TSharedPtr<IPropertyHandle> gridDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridDimensions));
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _fillGridWithCells));

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridOrientation));
	TSharedPtr<IPropertyHandle> gridVerticalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridVerticalAlignment));
	TSharedPtr<IPropertyHandle> gridHorizontalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridHorizontalAlignment));
	
	TSharedPtr<IPropertyHandle> rows = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _rows));
	TSharedPtr<IPropertyHandle> columns = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _columns));

	TSharedPtr<IPropertyHandle> useCellClassSizes = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellClassSizes));
	TSharedPtr<IPropertyHandle> cellSize = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellSize));	

	TSharedPtr<IPropertyHandle> cellTopMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellTopMargin));
	TSharedPtr<IPropertyHandle> cellLeftMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellLeftMargin));
	TSharedPtr<IPropertyHandle> cellRightMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellRightMargin));
	TSharedPtr<IPropertyHandle> cellBottomMargin = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellBottomMargin));
	
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

	bool itUsesCellsToShapeGrid;
	bool itFillsGridWithCells;
	EGridOrientation orientation;
	bool itUsesCellClassSizes;
	
	useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
	fillGridWithCells->GetValue(itFillsGridWithCells);
	gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation));
	useCellClassSizes->GetValue(itUsesCellClassSizes);

	if (itUsesCellsToShapeGrid)
	{
		DetailBuilder.HideProperty(gridDimensions);
		DetailBuilder.HideProperty(fillGridWithCells);
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

	if (!itUsesCellsToShapeGrid)
	{
		if (itFillsGridWithCells)
		{
			DetailBuilder.HideProperty(columns);
			DetailBuilder.HideProperty(rows);				
		}
		else
		{
			if (orientation == EGridOrientation::VERTICAL)
			{
				EGridVerticalAlignment verticalAlignment;
				gridVerticalAlignment->GetValue(reinterpret_cast<uint8&>(verticalAlignment));

				if (verticalAlignment == EGridVerticalAlignment::FILL)
				{
					DetailBuilder.HideProperty(cellTopMargin);
					DetailBuilder.HideProperty(cellBottomMargin);
				}	
			}
			else
			{
				EGridHorizontalAlignment horizontalAlignment;
				gridHorizontalAlignment->GetValue(reinterpret_cast<uint8&>(horizontalAlignment));

				if (horizontalAlignment == EGridHorizontalAlignment::FILL)
				{
					DetailBuilder.HideProperty(cellLeftMargin);
					DetailBuilder.HideProperty(cellRightMargin);
				}	
			}	
		}
	}	
}

#undef LOCTEXT_NAMESPACE