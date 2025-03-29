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

	TSharedPtr<IPropertyHandle> cellsCount = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellsCount));

	TSharedPtr<IPropertyHandle> useCellClassSizes = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _useCellClassSizes));
	TSharedPtr<IPropertyHandle> cellSize = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellSize));	

	TSharedPtr<IPropertyHandle> cellSpace = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGrid, _cellMargins));	

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

	DetailBuilder.EditDefaultProperty(gridVerticalAlignment)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid, gridOrientation, fillGridWithCells]()
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

	DetailBuilder.EditDefaultProperty(gridHorizontalAlignment)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid, gridOrientation, fillGridWithCells]()
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

	DetailBuilder.EditDefaultProperty(cellsCount)->Visibility(TAttribute<EVisibility>::Create([useCellsToShapeGrid, fillGridWithCells]()
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

	DetailBuilder.EditDefaultProperty(cellSpace)->Visibility(TAttribute<EVisibility>::Create(
		[useCellsToShapeGrid, fillGridWithCells, gridOrientation, gridVerticalAlignment, gridHorizontalAlignment]()
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

			EGridHorizontalAlignment horizontalAlignment;

			if (gridHorizontalAlignment->GetValue(reinterpret_cast<uint8&>(horizontalAlignment)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;
			}
			return horizontalAlignment == EGridHorizontalAlignment::FILL ? EVisibility::Collapsed : EVisibility::Visible;
		
		}));
	
}

#undef LOCTEXT_NAMESPACE