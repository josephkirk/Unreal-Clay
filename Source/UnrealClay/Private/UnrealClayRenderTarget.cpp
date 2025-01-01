#include "UnrealClayRenderTarget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "RenderUtils.h"

#include "ProfilingDebugging/CsvProfiler.h"

CSV_DEFINE_CATEGORY(UnrealClay, true);

UUnrealClayRenderTarget::UUnrealClayRenderTarget()
    : RenderTarget(nullptr)
    , Canvas(nullptr)
    , RenderTargetResource(nullptr)
    , bNeedsUpdate(true)
    , CachedDimensions(FIntPoint::ZeroValue)
    , CachedFormat(PF_Unknown)
    , DrawCalls(0)
    , LastDrawTime(0.0f)
{
}

void UUnrealClayRenderTarget::Initialize(const FUnrealClayRenderConfig& InConfig)
{
    Config = InConfig;
    bNeedsUpdate = true;
    UpdateRenderTarget();
}

void UUnrealClayRenderTarget::UpdateConfig(const FUnrealClayRenderConfig& InConfig)
{
    if (Config.Width != InConfig.Width || 
        Config.Height != InConfig.Height ||
        Config.bLinearColor != InConfig.bLinearColor ||
        Config.bEnableMSAA != InConfig.bEnableMSAA)
    {
        // Update config
        Config = InConfig;
        bNeedsUpdate = true;

        // Log update
        UE_LOG(LogTemp, Verbose, TEXT("UnrealClay: Updating render target config to %dx%d"), Config.Width, Config.Height);

        // Update render target
        UpdateRenderTarget();
    }
}

FCanvas* UUnrealClayRenderTarget::BeginDraw()
{
    CSV_SCOPED_TIMING_STAT(UnrealClay, BeginDraw);
    
    // Validate state
    if (!ValidateRenderTarget())
    {
        return nullptr;
    }

    // Get render target resource
    RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
    if (!RenderTargetResource)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnrealClay: Failed to get render target resource"));
        return nullptr;
    }

    // Create canvas
    Canvas = new FCanvas(
        RenderTargetResource,
        nullptr,
        GetWorld(),
        GMaxRHIFeatureLevel
    );

    // Clear render target
    const FLinearColor ClearColor = FLinearColor(0, 0, 0, 0);
    Canvas->Clear(ClearColor);

    // Reset stats
    DrawCalls = 0;
    LastDrawTime = 0.0f;

    return Canvas;
}

void UUnrealClayRenderTarget::EndDraw()
{
    CSV_SCOPED_TIMING_STAT(UnrealClay, EndDraw);
    
    if (Canvas)
    {
        const double StartTime = FPlatformTime::Seconds();

        // Flush canvas
        Canvas->Flush_GameThread();

        // Update stats
        LastDrawTime = FPlatformTime::Seconds() - StartTime;
        
        CSV_CUSTOM_STAT(UnrealClay, DrawCalls, DrawCalls, ECsvCustomStatOp::Set);
        CSV_CUSTOM_STAT(UnrealClay, DrawTime, LastDrawTime * 1000.0f, ECsvCustomStatOp::Set);

        // Cleanup
        delete Canvas;
        Canvas = nullptr;
    }

    RenderTargetResource = nullptr;
}

void UUnrealClayRenderTarget::UpdateRenderTarget()
{
    if (!bNeedsUpdate)
    {
        return;
    }

    // Validate dimensions
    Config.Width = FMath::Clamp(Config.Width, 1, 16384);
    Config.Height = FMath::Clamp(Config.Height, 1, 16384);

    // Create new render target if needed
    if (!RenderTarget)
    {
        RenderTarget = NewObject<UTextureRenderTarget2D>(this);
        if (!RenderTarget)
        {
            UE_LOG(LogTemp, Error, TEXT("UnrealClay: Failed to create render target"));
            return;
        }
    }

    // Get desired format
    const EPixelFormat DesiredFormat = Config.bLinearColor ? PF_FloatRGBA : PF_B8G8R8A8;

    // Check if we need to resize
    const bool bNeedsResize = 
        CachedDimensions.X != Config.Width ||
        CachedDimensions.Y != Config.Height ||
        CachedFormat != DesiredFormat;

    if (bNeedsResize)
    {
        // Update render target settings
        RenderTarget->InitCustomFormat(
            Config.Width,
            Config.Height,
            DesiredFormat,
            Config.bLinearColor
        );

        // Configure MSAA
        RenderTarget->TargetGamma = Config.bLinearColor ? 1.0f : 2.2f;
        RenderTarget->bForceLinearGamma = Config.bLinearColor;
        
        if (Config.bEnableMSAA)
        {
            static const auto CVarDefaultAntiAliasing = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.DefaultFeature.AntiAliasing"));
            const int32 DefaultAA = CVarDefaultAntiAliasing->GetValueOnGameThread();
            RenderTarget->AAMethod = DefaultAA == 2 ? EAntiAliasingMethod::AAM_MSAA : EAntiAliasingMethod::AAM_None;
            RenderTarget->NumSamples = 4;
        }
        else
        {
            RenderTarget->AAMethod = EAntiAliasingMethod::AAM_None;
            RenderTarget->NumSamples = 1;
        }

        // Update resource
        RenderTarget->UpdateResource();

        // Cache new dimensions
        CachedDimensions = FIntPoint(Config.Width, Config.Height);
        CachedFormat = DesiredFormat;

        // Fire resize callback
        OnRenderTargetResize();
        
        UE_LOG(LogTemp, Verbose, TEXT("UnrealClay: Resized render target to %dx%d"), Config.Width, Config.Height);
    }

    // Mark as updated
    bNeedsUpdate = false;
}

void UUnrealClayRenderTarget::OnRenderTargetResize()
{
    // Clear any cached resources that depend on render target size
}

bool UUnrealClayRenderTarget::ValidateRenderTarget() const
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnrealClay: Invalid render target"));
        return false;
    }

    if (Config.Width <= 0 || Config.Height <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnrealClay: Invalid render target dimensions: %dx%d"), Config.Width, Config.Height);
        return false;
    }

    return true;
}