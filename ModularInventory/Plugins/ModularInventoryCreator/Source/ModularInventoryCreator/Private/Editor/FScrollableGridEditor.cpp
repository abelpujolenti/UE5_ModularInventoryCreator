#include "FScrollableGridEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Grid/ScrollableGrid.h"

#define LOCTEXT_NAMESPACE "FScrollableGridEditor"

TSharedRef<IDetailCustomization> FScrollableGridEditor::MakeInstance()
{
	return MakeShareable(new FScrollableGridEditor);
}

void FScrollableGridEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{	
	TSharedPtr<IPropertyHandle> useCellsToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _useCellsToShapeGrid), UGrid::StaticClass());
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _fillGridWithCells), UGrid::StaticClass());

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _gridOrientation), UGrid::StaticClass());
	
	TSharedPtr<IPropertyHandle> rows = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _rows), UGrid::StaticClass());
	TSharedPtr<IPropertyHandle> columns = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScrollableGrid, _columns), UGrid::StaticClass());	

	DetailBuilder.EditDefaultProperty(columns)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation]()
		{			
			bool itUsesCellsToShapeGrid;
			
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);

			if (itUsesCellsToShapeGrid)
			{
				return EVisibility::Visible;
			}
			
			bool itFillsGridWithCells;
		
			fillGridWithCells->GetValue(itFillsGridWithCells);

			if (itFillsGridWithCells)
			{
				return EVisibility::Collapsed;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			return orientation == EGridOrientation::HORIZONTAL ? EVisibility::Visible : EVisibility::Collapsed;	
		}));

	DetailBuilder.EditDefaultProperty(rows)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation]()
		{			
			bool itUsesCellsToShapeGrid;
			
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);

			if (itUsesCellsToShapeGrid)
			{
				return EVisibility::Visible;
			}
			
			bool itFillsGridWithCells;
		
			fillGridWithCells->GetValue(itFillsGridWithCells);

			if (itFillsGridWithCells)
			{
				return EVisibility::Collapsed;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			return orientation == EGridOrientation::VERTICAL ? EVisibility::Visible : EVisibility::Collapsed;	
		}));
	
}

#undef LOCTEXT_NAMESPACE