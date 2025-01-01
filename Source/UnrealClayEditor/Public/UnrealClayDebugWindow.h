#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UnrealClayTypes.h"

class UUnrealClaySubsystem;

/**
 * Debug window for Clay UI system
 */
class SUnrealClayDebugWindow : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SUnrealClayDebugWindow)
    {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    /** Called when PIE starts */
    void OnPIEStarted(bool bIsSimulating);

    /** Called when PIE ends */
    void OnPIEEnded(bool bIsSimulating);

protected:
    /** Get Clay subsystem from active PIE world */
    UUnrealClaySubsystem* GetClaySubsystem() const;

    /** Draw performance stats */
    void DrawPerformanceStats() const;

    /** Draw memory usage */
    void DrawMemoryUsage() const;

    /** Draw debug options */
    void DrawDebugOptions();

private:
    /** Clay subsystem reference */
    TWeakObjectPtr<UUnrealClaySubsystem> ClaySubsystem;

    /** Debug settings */
    bool bShowPerformanceStats;
    bool bShowMemoryUsage;
    bool bShowDebugUI;
    bool bCaptureFrames;
};