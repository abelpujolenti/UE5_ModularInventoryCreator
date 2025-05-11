#include "FScrollPropertiesEditor.h"

#include "DetailLayoutBuilder.h"
#include "Inventory/Scroll/BaseScrollBarProperties.h"

#define LOCTEXT_NAMESPACE "FScrollPropertiesEditor"

TSharedRef<IDetailCustomization> FScrollPropertiesEditor::MakeInstance()
{
	return MakeShareable(new FScrollPropertiesEditor);
}

void FScrollPropertiesEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> snapToScrollable = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseScrollBarProperties, snapToScrollable));
	TSharedPtr<IPropertyHandle> positionRelativeToScrollablePivot = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseScrollBarProperties, positionRelativeToScrollablePivot));

	TSharedPtr<IPropertyHandle> adjustLengthToScrollable = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseScrollBarProperties, adjustLengthToScrollable));
	TSharedPtr<IPropertyHandle> length = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBaseScrollBarProperties, length));
	
	auto RefreshEditor = [&DetailBuilder]()
	{
		DetailBuilder.ForceRefreshDetails();
	};

	snapToScrollable->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));
	adjustLengthToScrollable->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(RefreshEditor));

	bool doesSnapToScrollable;
	bool doesAdjustLengthToScrollable;
	
	snapToScrollable->GetValue(doesSnapToScrollable);
	adjustLengthToScrollable->GetValue(doesAdjustLengthToScrollable);

	if (doesSnapToScrollable)
	{
		DetailBuilder.HideProperty(positionRelativeToScrollablePivot);
	}

	if (doesAdjustLengthToScrollable)
	{
		DetailBuilder.HideProperty(length);
	}
}

#undef LOCTEXT_NAMESPACE