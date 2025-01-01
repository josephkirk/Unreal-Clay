#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UnrealClayTypes.h"
#include "UnrealClayWidget.generated.h"

class SUnrealClayWidget;
class UUnrealClaySubsystem;

/**
 * Widget component for displaying Clay UI in UMG
 */
UCLASS()
class UNREALCLAY_API UUnrealClayWidget : public UWidget
{
    GENERATED_BODY()

public:
    UUnrealClayWidget(const FObjectInitializer& ObjectInitializer);

    //~ Begin UWidget Interface
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void SynchronizeProperties() override;
    //~ End UWidget Interface

    /** Configure render target settings */
    UFUNCTION(BlueprintCallable, Category = "UnrealClay")
    void SetRenderConfig(const FUnrealClayRenderConfig& Config);

    /** Get current render config */
    UFUNCTION(BlueprintPure, Category = "UnrealClay")
    const FUnrealClayRenderConfig& GetRenderConfig() const { return RenderConfig; }

    /** Event when element is clicked */
    UPROPERTY(BlueprintAssignable, Category = "UnrealClay")
    FOnClayEvent OnElementClicked;

    /** Event when element is hovered */
    UPROPERTY(BlueprintAssignable, Category = "UnrealClay")
    FOnClayEvent OnElementHovered;

protected:
    /** Native pointer to Clay subsystem */
    TWeakObjectPtr<UUnrealClaySubsystem> ClaySubsystem;

    /** Called when input is received */
    void HandleInput(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

    /** Called when mouse position changes */
    void HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

    /** Called when mouse button is pressed */
    void HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

    /** Called when mouse button is released */
    void HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

    /** Called when mouse wheel is scrolled */
    void HandleMouseWheel(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

    /** Convert geometry-relative coordinates to render target space */
    FVector2D GetLocalMousePosition(const FGeometry& Geometry, const FVector2D& ScreenSpacePosition) const;

private:
    /** Slate widget reference */
    TSharedPtr<SUnrealClayWidget> ClayWidget;

    /** Current render config */
    UPROPERTY(EditAnywhere, Category = "UnrealClay")
    FUnrealClayRenderConfig RenderConfig;

    /** Find and cache Clay subsystem */
    void InitializeClaySubsystem();

    /** Cached widget geometry for input handling */
    FGeometry CachedGeometry;

    /** Debug flags */
    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowDebugInfo;

    friend class SUnrealClayWidget;
};