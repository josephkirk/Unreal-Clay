#include "SlateUnrealClayWidget.h"
#include "UnrealClayWidget.h"
#include "UnrealClaySubsystem.h"
#include "UnrealClayRenderTarget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Text/STextBlock.h"

void SUnrealClayWidget::Construct(const FArguments& InArgs)
{
    ClayWidget = InArgs._ClayWidget;

    // Get Clay subsystem
    if (UGameInstance* GameInstance = ClayWidget->GetWorld()->GetGameInstance())
    {
        ClaySubsystem = GameInstance->GetSubsystem<UUnrealClaySubsystem>();
    }
}

void SUnrealClayWidget::UpdateRenderConfig(const FUnrealClayRenderConfig& Config)
{
    if (ClaySubsystem.IsValid())
    {
        ClaySubsystem->SetRenderConfig(Config);
    }
}

int32 SUnrealClayWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!ClaySubsystem.IsValid())
    {
        return LayerId;
    }

    // Get render target texture
    UTextureRenderTarget2D* RenderTarget = ClaySubsystem->GetRenderTarget()->GetRenderTarget();
    if (!RenderTarget)
    {
        return LayerId;
    }

    // Draw render target
    const FSlateLayoutTransform LayoutTransform(AllottedGeometry.GetAccumulatedLayoutTransform());
    const FSlateRenderTransform RenderTransform(AllottedGeometry.GetAccumulatedRenderTransform());

    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(RenderTransform),
        RenderTarget,
        ESlateDrawEffect::None,
        InWidgetStyle.GetColorAndOpacityTint()
    );

    // Draw debug info if enabled
    if (ClayWidget->bShowDebugInfo)
    {
        DrawDebugInfo(OutDrawElements, AllottedGeometry, LayerId + 1, InWidgetStyle);
    }

    return LayerId + (ClayWidget->bShowDebugInfo ? 2 : 1);
}

FVector2D SUnrealClayWidget::ComputeDesiredSize(float) const
{
    if (ClaySubsystem.IsValid())
    {
        const FUnrealClayRenderConfig& Config = ClaySubsystem->GetRenderTarget()->GetConfig();
        return FVector2D(Config.Width, Config.Height);
    }

    return FVector2D(100, 100);
}

FReply SUnrealClayWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (ClayWidget.IsValid())
    {
        ClayWidget->HandleMouseMove(MyGeometry, MouseEvent);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply SUnrealClayWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (ClayWidget.IsValid())
    {
        ClayWidget->HandleMouseButtonDown(MyGeometry, MouseEvent);
        return FReply::Handled().CaptureMouse(SharedThis(this));
    }

    return FReply::Unhandled();
}

FReply SUnrealClayWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (ClayWidget.IsValid())
    {
        ClayWidget->HandleMouseButtonUp(MyGeometry, MouseEvent);
        return FReply::Handled().ReleaseMouseCapture();
    }

    return FReply::Unhandled();
}

FReply SUnrealClayWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (ClayWidget.IsValid())
    {
        ClayWidget->HandleMouseWheel(MyGeometry, MouseEvent);
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FCursorReply SUnrealClayWidget::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
    return FCursorReply::Cursor(GetMouseCursor());
}

FVector2D SUnrealClayWidget::GetRenderPosition(const FGeometry& Geometry, const FVector2D& LocalPosition) const
{
    if (!ClaySubsystem.IsValid())
    {
        return FVector2D::ZeroVector;
    }

    const FUnrealClayRenderConfig& Config = ClaySubsystem->GetRenderTarget()->GetConfig();
    const FVector2D Size(Config.Width, Config.Height);
    const FVector2D Scale = Size / Geometry.GetLocalSize();

    return LocalPosition * Scale;
}

EMouseCursor::Type SUnrealClayWidget::GetMouseCursor() const
{
    // TODO: Implement cursor state based on Clay UI state
    return EMouseCursor::Default;
}

void SUnrealClayWidget::DrawDebugInfo(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, 
    int32 LayerId, const FWidgetStyle& InWidgetStyle) const
{
    if (!ClayWidget.IsValid() || !ClaySubsystem.IsValid())
    {
        return;
    }

    const FUnrealClayRenderConfig& Config = ClaySubsystem->GetRenderTarget()->GetConfig();
    
    // Background for debug info
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(0, 0), FVector2D(200, 120)),
        FEditorStyle::GetBrush("ToolPanel.GroupBorder"),
        ESlateDrawEffect::None,
        FLinearColor(0.0f, 0.0f, 0.0f, 0.75f)
    );

    // Debug text
    const float LineHeight = 20.0f;
    float YPosition = 10.0f;

    auto AddDebugText = [&](const FString& Label, const FString& Value)
    {
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 1,
            AllottedGeometry.ToPaintGeometry(FVector2D(10, YPosition), FVector2D(180, LineHeight)),
            FText::FromString(FString::Printf(TEXT("%s: %s"), *Label, *Value)),
            FCoreStyle::GetDefaultFontStyle("Regular", 9),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
        YPosition += LineHeight;
    };

    // Add debug info
    AddDebugText(TEXT("Size"), FString::Printf(TEXT("%dx%d"), Config.Width, Config.Height));
    AddDebugText(TEXT("MSAA"), Config.bEnableMSAA ? TEXT("Enabled") : TEXT("Disabled"));
    AddDebugText(TEXT("Color"), Config.bLinearColor ? TEXT("Linear") : TEXT("sRGB"));
    AddDebugText(TEXT("Memory"), TEXT("TODO"));  // TODO: Add memory usage stats
    AddDebugText(TEXT("Draw Calls"), TEXT("TODO"));  // TODO: Add draw call stats
}