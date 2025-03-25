#include "FGridEditor.h"

#include "DetailLayoutBuilder.h"
#include "Grid.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "FGridEditor"

TSharedRef<IDetailCustomization> FGridEditor::MakeInstance()
{
	return MakeShareable(new FGridEditor);
}

void FGridEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	/*DetailBuilder.EditCategory("Grid|Layout", FText::GetEmpty(), ECategoryPriority::Uncommon);
	DetailBuilder.EditCategory("Grid|Cells", FText::GetEmpty(), ECategoryPriority::Uncommon);

	DetailBuilder.SortCategories();*/
	
	TSharedPtr<IPropertyHandle> useCellToShapeGrid = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellsToShapeGrid));
	TSharedPtr<IPropertyHandle> gridDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridDimensions));
	TSharedPtr<IPropertyHandle> fillGridWithCells = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _fillGridWithCells));

	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridOrientation));
	TSharedPtr<IPropertyHandle> gridVerticalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridVerticalAlignment));
	TSharedPtr<IPropertyHandle> gridHorizontalAlignment = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _gridHorizontalAlignment));

	TSharedPtr<IPropertyHandle> cellsCount = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellsCount));

	TSharedPtr<IPropertyHandle> useCellClassSizes = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellClassSizes));
	TSharedPtr<IPropertyHandle> cellSize = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellSize));	

	DetailBuilder.EditDefaultProperty(gridDimensions)->Visibility(TAttribute<EVisibility>::Create([useCellToShapeGrid]()
		{
			bool enable;
			useCellToShapeGrid->GetValue(enable);
			return enable ? EVisibility::Collapsed : EVisibility::Visible;
		}));

	DetailBuilder.EditDefaultProperty(fillGridWithCells)->Visibility(TAttribute<EVisibility>::Create([useCellToShapeGrid]()
		{
			bool enable;
			useCellToShapeGrid->GetValue(enable);
			return enable ? EVisibility::Collapsed : EVisibility::Visible;			
		}));

	DetailBuilder.EditDefaultProperty(gridVerticalAlignment)->Visibility(TAttribute<EVisibility>::Create([gridOrientation]()
		{
			GridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}
			return orientation == GridOrientation::VERTICAL ? EVisibility::Visible : EVisibility::Collapsed;
		}));

	DetailBuilder.EditDefaultProperty(gridHorizontalAlignment)->Visibility(TAttribute<EVisibility>::Create([gridOrientation]()
		{
			GridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}
			return orientation == GridOrientation::HORIZONTAL ? EVisibility::Visible : EVisibility::Collapsed;
		}));

	DetailBuilder.EditDefaultProperty(cellsCount)->Visibility(TAttribute<EVisibility>::Create([useCellToShapeGrid, fillGridWithCells]()
		{
			bool enable1;
			bool enable2;
			useCellToShapeGrid->GetValue(enable1);
			fillGridWithCells->GetValue(enable2);
			return enable1 || !enable2 ? EVisibility::Visible : EVisibility::Collapsed;			
		}));

	DetailBuilder.EditDefaultProperty(cellSize)->Visibility(TAttribute<EVisibility>::Create([useCellClassSizes]()
		{
			bool enable;
			useCellClassSizes->GetValue(enable);
			return enable ? EVisibility::Collapsed : EVisibility::Visible;			
		}));
	
}

#undef LOCTEXT_NAMESPACE