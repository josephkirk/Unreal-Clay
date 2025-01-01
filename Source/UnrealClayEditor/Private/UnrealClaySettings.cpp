#include "UnrealClaySettings.h"

UUnrealClaySettings::UUnrealClaySettings()
{
    // Set default values
    DefaultRenderConfig.Width = 1920;
    DefaultRenderConfig.Height = 1080;
    DefaultRenderConfig.bLinearColor = false;
    DefaultRenderConfig.bEnableMSAA = true;

    bAutoCompileShaders = true;
    bEnableDebugFeatures = true;
    bEnablePerformanceMonitoring = true;
    MaxElementsPerView = 8192;
    MemoryPoolSizeMB = 64;
    DefaultFontFamily = TEXT("Roboto");
}