#include "UnrealClayDebugWindow.h"
#include "UnrealClaySubsystem.h"
#include "UnrealClayTypes.h"
#include "Editor.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "UnrealClayDebugWindow"

void SUnrealClayDebugWindow::Construct(const FArguments& InArgs)
{
    bShowPerformanceStats = true;
    bShowMemoryUsage = true;
    bShowDebugUI = true;
    bCaptureFrames = false;

    ChildSlot
    [
        SNew(SVerticalBox)

        // Title
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(8)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("DebugWindowTitle", "Clay UI Debugger"))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
        ]

        // Debug options
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(8)
        [
            SNew(SVerticalBox)
            
            // Performance stats toggle
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SCheckBox)
                .IsChecked(bShowPerformanceStats ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                {
                    bShowPerformanceStats = NewState == ECheckBoxState::Checked;
                })
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ShowPerformanceStats", "Show Performance Stats"))
                ]
            ]

            // Memory usage toggle
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SCheckBox)
                .IsChecked(bShowMemoryUsage ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                {
                    bShowMemoryUsage = NewState == ECheckBoxState::Checked;
                })
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ShowMemoryUsage", "Show Memory Usage"))
                ]
            ]

            // Debug UI toggle
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SCheckBox)
                .IsChecked(bShowDebugUI ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                {
                    bShowDebugUI = NewState == ECheckBoxState::Checked;
                    if (UUnrealClaySubsystem* Subsystem = GetClaySubsystem())
                    {
                        // TODO: Add debug UI toggle to subsystem
                    }
                })
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ShowDebugUI", "Show Debug UI"))
                ]
            ]

            // Frame capture toggle
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SCheckBox)
                .IsChecked(bCaptureFrames ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                {
                    bCaptureFrames = NewState == ECheckBoxState::Checked;
                })
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("CaptureFrames", "Capture Frames"))
                ]
            ]
        ]

        // Stats area
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .Padding(8)
        [
            SNew(SScrollBox)

            // Performance stats
            + SScrollBox::Slot()
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 4)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("PerformanceStats", "Performance Stats"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                    .Visibility_Lambda([this]() { return bShowPerformanceStats ? EVisibility::Visible : EVisibility::Collapsed; })
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8, 4)
                [
                    SNew(STextBlock)
                    .Text_Lambda([this]()
                    {
                        if (UUnrealClaySubsystem* Subsystem = GetClaySubsystem())
                        {
                            // TODO: Get performance stats from subsystem
                            return FText::FromString(TEXT("Frame Time: 0.0ms\nDraw Calls: 0\nElements: 0"));
                        }
                        return FText::GetEmpty();
                    })
                    .Visibility_Lambda([this]() { return bShowPerformanceStats ? EVisibility::Visible : EVisibility::Collapsed; })
                ]
            ]

            // Memory usage
            + SScrollBox::Slot()
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 4)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("MemoryUsage", "Memory Usage"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                    .Visibility_Lambda([this]() { return bShowMemoryUsage ? EVisibility::Visible : EVisibility::Collapsed; })
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8, 4)
                [
                    SNew(STextBlock)
                    .Text_Lambda([this]()
                    {
                        if (UUnrealClaySubsystem* Subsystem = GetClaySubsystem())
                        {
                            // TODO: Get memory usage from subsystem
                            return FText::FromString(TEXT("Total Memory: 0 MB\nPeak Memory: 0 MB\nAllocations: 0"));
                        }
                        return FText::GetEmpty();
                    })
                    .Visibility_Lambda([this]() { return bShowMemoryUsage ? EVisibility::Visible : EVisibility::Collapsed; })
                ]
            ]
        ]
    ];
}

void SUnrealClayDebugWindow::OnPIEStarted(bool bIsSimulating)
{
    if (!bIsSimulating)
    {
        ClaySubsystem = GetClaySubsystem();
    }
}

void SUnrealClayDebugWindow::OnPIEEnded(bool bIsSimulating)
{
    if (!bIsSimulating)
    {
        ClaySubsystem.Reset();
    }
}

UUnrealClaySubsystem* SUnrealClayDebugWindow::GetClaySubsystem() const
{
    if (GEditor)
    {
        if (UWorld* PIEWorld = GEditor->GetPIEWorldContext()->World())
        {
            if (UGameInstance* GameInstance = PIEWorld->GetGameInstance())
            {
                return GameInstance->GetSubsystem<UUnrealClaySubsystem>();
            }
        }
    }
    return nullptr;
}

#undef LOCTEXT_NAMESPACE