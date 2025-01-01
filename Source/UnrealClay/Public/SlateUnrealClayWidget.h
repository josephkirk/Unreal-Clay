#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UnrealClayTypes.h"

class UUnrealClayWidget;
class UUnrealClaySubsystem;

/**
 * Slate widget that renders Clay UI
 */
class SUnrealClayWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SUnrealClayWidget)
        : _ClayWidget(nullptr)
    {}
        SLATE_ARGUMENT(UUnrealClayWidget*, ClayWidget)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    /**
     * Update render config settings
     * @param Config - New render configuration
     */
    void UpdateRenderConfig(const FUnrealClayRenderConfig& Config);

protected:
    //~ Begin SWidget Interface
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override;
    virtual bool SupportsKeyboardFocus() const override { return true; }
    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
    //~ End SWidget Interface

private:
    /** Owner UMG widget */
    TWeakObjectPtr<UUnrealClayWidget> ClayWidget;

    /** Clay subsystem reference */
    TWeakObjectPtr<UUnrealClaySubsystem> ClaySubsystem;

    /** Convert local coordinates to render target space */
    FVector2D GetRenderPosition(const FGeometry& Geometry, const FVector2D& LocalPosition) const;

    /** Get current mouse cursor */
    EMouseCursor::Type GetMouseCursor() const;

    /** Debug drawing */
    void DrawDebugInfo(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, 
        int32 LayerId, const FWidgetStyle& InWidgetStyle) const;
};