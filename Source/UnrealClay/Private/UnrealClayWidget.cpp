#include "UnrealClayWidget.h"
#include "SlateUnrealClayWidget.h"
#include "UnrealClaySubsystem.h"

UUnrealClayWidget::UUnrealClayWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bShowDebugInfo(false)
{
    bIsVariable = false;
    SetVisibility(ESlateVisibility::Visible);
}

void UUnrealClayWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    ClayWidget.Reset();
}

TSharedRef<SWidget> UUnrealClayWidget::RebuildWidget()
{
    // Initialize subsystem
    InitializeClaySubsystem();

    // Create Slate widget
    ClayWidget = SNew(SUnrealClayWidget)
        .ClayWidget(this);

    return ClayWidget.ToSharedRef();
}

void UUnrealClayWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (ClayWidget.IsValid())
    {
        ClayWidget->UpdateRenderConfig(RenderConfig);
    }
}

void UUnrealClayWidget::SetRenderConfig(const FUnrealClayRenderConfig& Config)
{
    RenderConfig = Config;

    if (ClayWidget.IsValid())
    {
        ClayWidget->UpdateRenderConfig(RenderConfig);
    }
}

void UUnrealClayWidget::InitializeClaySubsystem()
{
    if (!ClaySubsystem.IsValid())
    {
        if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
        {
            ClaySubsystem = GameInstance->GetSubsystem<UUnrealClaySubsystem>();
        }
    }
}

void UUnrealClayWidget::HandleInput(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
    if (!ClaySubsystem.IsValid())
    {
        return;
    }

    CachedGeometry = Geometry;

    // Update input state
    const FVector2D LocalPosition = GetLocalMousePosition(Geometry, MouseEvent.GetScreenSpacePosition());
    const bool bIsLeftButtonDown = MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
    ClaySubsystem->UpdateInput(LocalPosition, bIsLeftButtonDown, FVector2D::ZeroVector);
}

void UUnrealClayWidget::HandleMouseMove(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
    HandleInput(Geometry, MouseEvent);
}

void UUnrealClayWidget::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
    HandleInput(Geometry, MouseEvent);
}

void UUnrealClayWidget::HandleMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
    HandleInput(Geometry, MouseEvent);
}

void UUnrealClayWidget::HandleMouseWheel(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
    if (!ClaySubsystem.IsValid())
    {
        return;
    }

    // Update scroll state
    const FVector2D LocalPosition = GetLocalMousePosition(Geometry, MouseEvent.GetScreenSpacePosition());
    const FVector2D ScrollDelta(0.0f, MouseEvent.GetWheelDelta());
    const bool bIsLeftButtonDown = MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
    ClaySubsystem->UpdateInput(LocalPosition, bIsLeftButtonDown, ScrollDelta);
}

FVector2D UUnrealClayWidget::GetLocalMousePosition(const FGeometry& Geometry, const FVector2D& ScreenSpacePosition) const
{
    const FVector2D LocalPosition = Geometry.AbsoluteToLocal(ScreenSpacePosition);
    const FVector2D Size(RenderConfig.Width, RenderConfig.Height);
    const FVector2D Scale = Size / Geometry.GetLocalSize();
    return LocalPosition * Scale;
}