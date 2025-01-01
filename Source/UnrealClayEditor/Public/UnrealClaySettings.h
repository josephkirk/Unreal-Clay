#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnrealClayTypes.h"
#include "UnrealClaySettings.generated.h"

/**
 * Settings for the UnrealClay plugin
 */
UCLASS(config=Editor, defaultconfig)
class UNREALCLAYEDITOR_API UUnrealClaySettings : public UObject
{
    GENERATED_BODY()

public:
    UUnrealClaySettings();

    /** Default render target settings */
    UPROPERTY(config, EditAnywhere, Category = "Default Settings")
    FUnrealClayRenderConfig DefaultRenderConfig;

    /** Enable automatic shader compilation */
    UPROPERTY(config, EditAnywhere, Category = "Development")
    bool bAutoCompileShaders;

    /** Enable debug features in development builds */
    UPROPERTY(config, EditAnywhere, Category = "Development")
    bool bEnableDebugFeatures;

    /** Enable performance monitoring */
    UPROPERTY(config, EditAnywhere, Category = "Performance")
    bool bEnablePerformanceMonitoring;

    /** Maximum elements per view */
    UPROPERTY(config, EditAnywhere, Category = "Performance")
    int32 MaxElementsPerView;

    /** Memory pool size in megabytes */
    UPROPERTY(config, EditAnywhere, Category = "Memory", meta=(ClampMin="1", ClampMax="1024"))
    int32 MemoryPoolSizeMB;

    /** Font directory override */
    UPROPERTY(config, EditAnywhere, Category = "Assets", meta=(RelativeToGameContentDir))
    FDirectoryPath FontDirectory;

    /** Default font family */
    UPROPERTY(config, EditAnywhere, Category = "Assets")
    FString DefaultFontFamily;
};