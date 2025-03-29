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

	DetailBuilder.EditDefaultProperty(gridDimensions)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid]()
		{
			bool itUsesCellsToShapeGrid;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
		
			return itUsesCellsToShapeGrid ? EVisibility::Collapsed : EVisibility::Visible;
		}));

	DetailBuilder.EditDefaultProperty(fillGridWithCells)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid]()
		{
			bool itUsesCellsToShapeGrid;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
		
			return itUsesCellsToShapeGrid ? EVisibility::Collapsed : EVisibility::Visible;			
		}));

	DetailBuilder.EditDefaultProperty(gridVerticalAlignment)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, gridOrientation, fillGridWithCells]()
	{
			bool itUsesCellsToShapeGrid;
			bool itFillsGridWithCells;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
			fillGridWithCells->GetValue(itFillsGridWithCells);
		
			if (itUsesCellsToShapeGrid ||itFillsGridWithCells)
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

	DetailBuilder.EditDefaultProperty(gridHorizontalAlignment)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, gridOrientation, fillGridWithCells]()
	{
			bool itUsesCellsToShapeGrid;
			bool itFillsGridWithCells;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
			fillGridWithCells->GetValue(itFillsGridWithCells);
		
			if (itUsesCellsToShapeGrid || itFillsGridWithCells)
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

	DetailBuilder.EditDefaultProperty(columns)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid, fillGridWithCells]()
		{
			bool itUsesCellsToShapeGrid;
			bool itFillsGridWithCells;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
			fillGridWithCells->GetValue(itFillsGridWithCells);
		
			return itUsesCellsToShapeGrid || !itFillsGridWithCells ? EVisibility::Visible : EVisibility::Collapsed;			
		}));

	DetailBuilder.EditDefaultProperty(rows)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid, fillGridWithCells]()
		{
			bool itUsesCellsToShapeGrid;
			bool itFillsGridWithCells;
		
			useCellsToShapeGrid->GetValue(itUsesCellsToShapeGrid);
			fillGridWithCells->GetValue(itFillsGridWithCells);
		
			return itUsesCellsToShapeGrid || !itFillsGridWithCells ? EVisibility::Visible : EVisibility::Collapsed;			
		}));

	DetailBuilder.EditDefaultProperty(cellSize)->Visibility(TAttribute<EVisibility>::Create([useCellClassSizes]()
		{
			bool itUsesCellsToShapeGrid;
			useCellClassSizes->GetValue(itUsesCellsToShapeGrid);
			return itUsesCellsToShapeGrid ? EVisibility::Collapsed : EVisibility::Visible;			
		}));	

	DetailBuilder.EditDefaultProperty(cellTopMargin)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation, gridVerticalAlignment]()
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
				return EVisibility::Visible;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			if (orientation == EGridOrientation::VERTICAL)
			{
				EGridVerticalAlignment verticalAlignment;

				if (gridVerticalAlignment->GetValue(reinterpret_cast<uint8&>(verticalAlignment)) != FPropertyAccess::Success)
				{
					return EVisibility::Collapsed;			
				}

				return verticalAlignment == EGridVerticalAlignment::FILL ? EVisibility::Collapsed : EVisibility::Visible; 
			}
			
			return EVisibility::Visible;		
		}));
	

	DetailBuilder.EditDefaultProperty(cellBottomMargin)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation, gridVerticalAlignment]()
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
				return EVisibility::Visible;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			if (orientation == EGridOrientation::VERTICAL)
			{
				EGridVerticalAlignment verticalAlignment;

				if (gridVerticalAlignment->GetValue(reinterpret_cast<uint8&>(verticalAlignment)) != FPropertyAccess::Success)
				{
					return EVisibility::Collapsed;			
				}

				return verticalAlignment == EGridVerticalAlignment::FILL ? EVisibility::Collapsed : EVisibility::Visible; 
			}

			return EVisibility::Visible;		
		}));
	

	DetailBuilder.EditDefaultProperty(cellLeftMargin)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation, gridHorizontalAlignment]()
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
				return EVisibility::Visible;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			if (orientation == EGridOrientation::VERTICAL)
			{
				return EVisibility::Visible;
			}

			EGridHorizontalAlignment horizontalAlignment;

			if (gridHorizontalAlignment->GetValue(reinterpret_cast<uint8&>(horizontalAlignment)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;
			}
			return horizontalAlignment == EGridHorizontalAlignment::FILL ? EVisibility::Collapsed : EVisibility::Visible;
		
		}));
	

	DetailBuilder.EditDefaultProperty(cellRightMargin)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation, gridHorizontalAlignment]()
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
				return EVisibility::Visible;
			}
			
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;				
			}

			if (orientation == EGridOrientation::VERTICAL)
			{
				return EVisibility::Visible;
			}

			EGridHorizontalAlignment horizontalAlignment;

			if (gridHorizontalAlignment->GetValue(reinterpret_cast<uint8&>(horizontalAlignment)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;
			}
			
			return horizontalAlignment == EGridHorizontalAlignment::FILL ? EVisibility::Collapsed : EVisibility::Visible;
		
		}));
	
}

#undef LOCTEXT_NAMESPACE