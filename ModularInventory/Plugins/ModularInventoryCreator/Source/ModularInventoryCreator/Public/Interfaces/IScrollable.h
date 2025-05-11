#pragma once
#include "Inventory/Grid/Grid.h"

#include "IScrollable.generated.h"

template <typename TKey, typename TValue>
class ObserverMap;

UENUM()
enum EScrollKeys
{
	DISPLACEMENT,
	MAXIMUM_DISPLACEMENT
};

UINTERFACE(MinimalAPI)
class UIScrollable : public UInterface
{
	GENERATED_BODY()
};

class MODULARINVENTORYCREATOR_API IIScrollable
{

public:

	UFUNCTION(BlueprintNativeEvent)
	void Scroll(float deltaDistance);

	UFUNCTION(BlueprintNativeEvent)
	FVector2D GetPivot() const;	
	UFUNCTION(BlueprintNativeEvent)
	EGridOrientation GetOrientation() const;
	
	UFUNCTION(BlueprintNativeEvent)
	float GetWidth() const;	
	UFUNCTION(BlueprintNativeEvent)
	float GetHeight() const;	
	UFUNCTION(BlueprintNativeEvent)
	float GetLength() const;

	UFUNCTION(BlueprintNativeEvent)
	float GetMaximumDisplacement() const;
	
	virtual ObserverMap<EScrollKeys, float>* const GetScrollObserver() const = 0;

private:
	GENERATED_BODY()
};
