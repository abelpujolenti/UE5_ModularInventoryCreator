#pragma once
#include "Inventory/Grid/Grid.h"

#include "IScrollable.generated.h"

UINTERFACE(MinimalAPI)
class UIScrollable : public UInterface
{
	GENERATED_BODY()
};

class MODULARINVENTORYCREATOR_API IIScrollable
{

public:

	virtual void Scroll(float deltaDistance) = 0;

	virtual EGridOrientation GetOrientation() const = 0;

	virtual float GetLength() const = 0;

	virtual float GetMaximumDisplacement() const = 0;

private:
	GENERATED_BODY()
};
