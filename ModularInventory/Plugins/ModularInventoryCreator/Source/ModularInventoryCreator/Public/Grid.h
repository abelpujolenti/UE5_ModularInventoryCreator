// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "InventoryWidgetBase.h"
#include "Grid.generated.h"

class UCanvasPanelSlot;
class USizeBoxSlot;
class USizeBox;
class UCanvasPanel;
class UGridPanel;
class UCell;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FRowCol
{	
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (DisplayName = "Row", ClampMin = 1, UIMin = 1))
	int32 row;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (DisplayName = "Column", ClampMin = 1, UIMin = 1))
	int32 column;
	
	GENERATED_BODY();	
};

UENUM(BlueprintType)
enum class GridOrientation : uint8
{
	VERTICAL UMETA(DisplayName = "Vertical"),
	HORIZONTAL UMETA(DisplayName = "Horizontal")
};

UENUM(BlueprintType)
enum class GridVerticalAlignment : uint8
{
	FILL UMETA(DisplayName = "Fill"),
	TOP UMETA(DisplayName = "Top"),
	CENTER UMETA(DisplayName = "Center"),
	BOTTOM UMETA(DisplayName = "Bottom")
};

UENUM(BlueprintType)
enum class GridHorizontalAlignment : uint8
{
	FILL UMETA(DisplayName = "Fill"),
	LEFT UMETA(DisplayName = "Left"),
	CENTER UMETA(DisplayName = "Center"),
	RIGHT UMETA(DisplayName = "Right")
};

UCLASS()
class MODULARINVENTORYCREATOR_API UGrid : public UInventoryWidgetBase, public IDetailCustomization
{

	friend class FGridEditor;
	
public:

#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:

	UPROPERTY(Transient, meta = (HideInDetailPanel))
	bool _isOrientationVertical;

protected:

	virtual void NativeOnInitialized() override;
	
	void InitGrid(const TObjectPtr<UWorld>& world);

	void CreateBoundaries(const TObjectPtr<UWorld>& world);
	
	virtual void CreateVerticalGrid(const TObjectPtr<UWorld>& world, const int& minXBounds, const int& maxXBounds,
		int& currentXPosition, const int& maxYBounds, int& currentYPosition);
	
	virtual void CreateHorizontalGrid(const TObjectPtr<UWorld>& world, const int& maxXBounds, int& currentXPosition,
		const int& minYBounds, const int& maxYBounds, int& currentYPosition);

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> _sizeBoxSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> _sizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> _canvas = nullptr;

	#pragma region Layout

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Layout")
	FVector2D _gridPivot;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Layout")
	bool _useCellsToShapeGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FVector2D _gridDimensions;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Layout", meta = (ClampMin = 1, UIMin = 1))
	FMargin _gridPadding;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Layout")
	GridOrientation _gridOrientation;

	#pragma endregion

	#pragma region Cells

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells")
	TSubclassOf<UCell> _cellClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells")
	bool _useCellClassSizes;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells")
	bool _fillGridWithCells;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells")
	GridVerticalAlignment _gridVerticalAlignment;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells")
	GridHorizontalAlignment _gridHorizontalAlignment;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells", meta = (ClampMin = 1, UIMin = 1))
	FRowCol _cellsCount;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells",meta = (ClampMin = 1, UIMin = 1))
	FVector2D _cellSize;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Cells", meta = (ClampMin = 0, UIMin = 0))
	float _cellsSpace;

	TArray<TArray<TObjectPtr<UCell>>> _cellGrid;

	#pragma endregion

private:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	
	GENERATED_BODY()
};