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

UINTERFACE(MinimalAPI, BlueprintType)
class UIScrollable : public UInterface
{
	GENERATED_BODY()
};

class MODULARINVENTORYCREATOR_API IIScrollable
{

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetScroll();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Scroll(float deltaDistance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector2D GetPivot() const;	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EGridOrientation GetOrientation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetWidth() const;	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetHeight() const;	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetLength() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaximumDisplacement() const;
	
	virtual ObserverMap<EScrollKeys, float>* const GetScrollObserver() const = 0;

private:
	GENERATED_BODY()
};
