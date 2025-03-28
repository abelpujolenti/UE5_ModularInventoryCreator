#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"


class FGridEditor : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;	
};
