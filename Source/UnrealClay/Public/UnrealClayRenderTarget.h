#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnrealClayTypes.h"
#include "UnrealClayRenderTarget.generated.h"

class UTextureRenderTarget2D;
class FCanvas;

/**
 * Manages render target for Clay UI rendering
 */
UCLASS()
class UNREALCLAY_API UUnrealClayRenderTarget : public UObject
{
    GENERATED_BODY()

public:
    UUnrealClayRenderTarget();

    /** Initialize render target with config */
    void Initialize(const FUnrealClayRenderConfig& InConfig);

    /** Update render target config */
    void UpdateConfig(const FUnrealClayRenderConfig& InConfig);

    /** Get current render target */
    UFUNCTION(BlueprintPure, Category = "UnrealClay")
    UTextureRenderTarget2D* GetRenderTarget() const { return RenderTarget; }

    /** Begin drawing to render target */
    FCanvas* BeginDraw();

    /** End drawing to render target */
    void EndDraw();

    /** Get current render config */
    const FUnrealClayRenderConfig& GetConfig() const { return Config; }

protected:
    /** Create or update render target based on current config */
    void UpdateRenderTarget();

    /** Called when render target needs resize */
    void OnRenderTargetResize();

    /** Validate render target state */
    bool ValidateRenderTarget() const;

private:
    /** Current render target */
    UPROPERTY()
    UTextureRenderTarget2D* RenderTarget;

    /** Current render config */
    FUnrealClayRenderConfig Config;

    /** Canvas for drawing */
    FCanvas* Canvas;

    /** Render target resource */
    FRenderTarget* RenderTargetResource;

    /** Whether render target needs update */
    bool bNeedsUpdate;

    /** Cached dimension to detect resizes */
    FIntPoint CachedDimensions;

    /** Cached render target format */
    EPixelFormat CachedFormat;

    /** Debug stats */
    uint32 DrawCalls;
    float LastDrawTime;

    friend class FUnrealClayRenderTargetSceneProxy;
};