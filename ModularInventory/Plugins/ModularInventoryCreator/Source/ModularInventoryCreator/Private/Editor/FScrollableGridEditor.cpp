#include "FScrollableGridEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Grid/ScrollableGrid.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "FScrollableGridEditor"

TSharedRef<IDetailCustomization> FScrollableGridEditor::MakeInstance()
{
	return MakeShareable(new FScrollableGridEditor);
}

void FScrollableGridEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> isScrollable = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _isScrollable));

	TSharedPtr<IPropertyHandle> extraLines = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _extraLines));
	TSharedPtr<IPropertyHandle> scroll = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _scroll));
	
	TSharedPtr<IPropertyHandle> useCellsToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _useCellsToShapeGrid), UGrid::StaticClass());
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _fillGridWithCells), UGrid::StaticClass());

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _gridOrientation), UGrid::StaticClass());
	
	TSharedPtr<IPropertyHandle> rows = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _rows), UGrid::StaticClass());
	TSharedPtr<IPropertyHandle> columns = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _columns), UGrid::StaticClass());
	

	auto RefreshEditor = [&DetailBuilder]()
	{
		DetailBuilder.ForceRefreshDetails();
	};

	isScrollable->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));

	bool enableScrollFields;
	isScrollable->GetValue(enableScrollFields);

	if (!enableScrollFields)
	{
		DetailBuilder.HideProperty(extraLines);
		DetailBuilder.HideProperty(scroll);
		return;
	}

	gridOrientation->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));

	bool itUsesCellsToShapeGrid;
	bool itFillsGridWithCells;
	EGridOrientation orientation;
			
	useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
	fillGridWithCells->GetValue(itFillsGridWithCells);

	if (!itUsesCellsToShapeGrid)
	{
		if (itFillsGridWithCells || gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
		{
			DetailBuilder.HideProperty(rows);
			DetailBuilder.HideProperty(columns);
		}
		else
		{
			if (orientation == EGridOrientation::VERTICAL)
			{
				DetailBuilder.HideProperty(columns);				
			}
			else
			{
				DetailBuilder.HideProperty(rows);	
			}					
		}	
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
}

#undef LOCTEXT_NAMESPACE