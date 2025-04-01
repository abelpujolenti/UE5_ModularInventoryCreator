#include "FScrollEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Grid/ScrollableGrid.h"
#include "Inventory/Scroll/Scroll.h"

#define LOCTEXT_NAMESPACE "FScrollGridEditor"

TSharedRef<IDetailCustomization> FScrollEditor::MakeInstance()
{
	return MakeShareable(new FScrollEditor);
}

void FScrollEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{	
	TSharedPtr<IPropertyHandle> gridOrientation = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScroll, _gridOrientation));
	TSharedPtr<IPropertyHandle> verticalOrientationPosition = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScroll, _verticalOrientationPosition));
	TSharedPtr<IPropertyHandle> horizontalOrientationPosition = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScroll, _horizontalOrientationPosition));

	DetailBuilder.EditDefaultProperty(verticalOrientationPosition)->Visibility(TAttribute<EVisibility>::Create([gridOrientation]()
		{
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;
			}
			return orientation == EGridOrientation::VERTICAL ? EVisibility::Visible : EVisibility::Collapsed;
		}));

	DetailBuilder.EditDefaultProperty(verticalOrientationPosition)->Visibility(TAttribute<EVisibility>::Create([gridOrientation]()
		{
			EGridOrientation orientation;
			if (gridOrientation->GetValue(reinterpret_cast<uint8&>(orientation)) != FPropertyAccess::Success)
			{
				return EVisibility::Collapsed;
			}
			return orientation == EGridOrientation::HORIZONTAL ? EVisibility::Visible : EVisibility::Collapsed;		
		}));
}

#undef LOCTEXT_NAMESPACE