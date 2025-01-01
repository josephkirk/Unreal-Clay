    const Clay_CornerRadius& CornerRadius)
{
    CSV_SCOPED_TIMING_STAT(ClaySubsystem, DrawRoundedRect);

    // Draw center rectangle
    const float MaxRadius = FMath::Max(
        FMath::Max(CornerRadius.topLeft, CornerRadius.topRight),
        FMath::Max(CornerRadius.bottomLeft, CornerRadius.bottomRight)
    );

    // Center rectangle
    {
        const FVector2D CenterPos(Position.X + MaxRadius, Position.Y);
        const FVector2D CenterSize(Size.X - MaxRadius * 2, Size.Y);
        FCanvasTileItem CenterTile(CenterPos, CenterSize, Color);
        CenterTile.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(CenterTile);
    }

    // Left rectangle
    if (CornerRadius.topLeft > 0 || CornerRadius.bottomLeft > 0)
    {
        const float LeftRadius = FMath::Max(CornerRadius.topLeft, CornerRadius.bottomLeft);
        const FVector2D LeftPos(Position.X, Position.Y + CornerRadius.topLeft);
        const FVector2D LeftSize(LeftRadius, Size.Y - CornerRadius.topLeft - CornerRadius.bottomLeft);
        FCanvasTileItem LeftTile(LeftPos, LeftSize, Color);
        LeftTile.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(LeftTile);
    }

    // Right rectangle
    if (CornerRadius.topRight > 0 || CornerRadius.bottomRight > 0)
    {
        const float RightRadius = FMath::Max(CornerRadius.topRight, CornerRadius.bottomRight);
        const FVector2D RightPos(Position.X + Size.X - RightRadius, Position.Y + CornerRadius.topRight);
        const FVector2D RightSize(RightRadius, Size.Y - CornerRadius.topRight - CornerRadius.bottomRight);
        FCanvasTileItem RightTile(RightPos, RightSize, Color);
        RightTile.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(RightTile);
    }

    // Draw rounded corners
    const int32 NumSegments = 8; // Number of segments per corner

    // Top left corner
    if (CornerRadius.topLeft > 0)
    {
        DrawCornerArc(Canvas, 
            Position + FVector2D(CornerRadius.topLeft, CornerRadius.topLeft),
            CornerRadius.topLeft, Color, NumSegments, 180.0f, 270.0f);
    }

    // Top right corner
    if (CornerRadius.topRight > 0)
    {
        DrawCornerArc(Canvas,
            Position + FVector2D(Size.X - CornerRadius.topRight, CornerRadius.topRight),
            CornerRadius.topRight, Color, NumSegments, 270.0f, 360.0f);
    }

    // Bottom right corner
    if (CornerRadius.bottomRight > 0)
    {
        DrawCornerArc(Canvas,
            Position + FVector2D(Size.X - CornerRadius.bottomRight, Size.Y - CornerRadius.bottomRight),
            CornerRadius.bottomRight, Color, NumSegments, 0.0f, 90.0f);
    }

    // Bottom left corner
    if (CornerRadius.bottomLeft > 0)
    {
        DrawCornerArc(Canvas,
            Position + FVector2D(CornerRadius.bottomLeft, Size.Y - CornerRadius.bottomLeft),
            CornerRadius.bottomLeft, Color, NumSegments, 90.0f, 180.0f);
    }
}

void UUnrealClaySubsystem::DrawCornerArc(FCanvas* Canvas, const FVector2D& Center, float Radius, 
    const FLinearColor& Color, int32 NumSegments, float StartAngle, float EndAngle)
{
    CSV_SCOPED_TIMING_STAT(ClaySubsystem, DrawCornerArc);

    const float AngleStep = (EndAngle - StartAngle) / NumSegments;
    TArray<FVector2D> Points;
    Points.Reserve(NumSegments + 2);

    // Add center point
    Points.Add(Center);

    // Generate arc points
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        const float Angle = FMath::DegreesToRadians(StartAngle + i * AngleStep);
        Points.Add(Center + FVector2D(
            FMath::Cos(Angle) * Radius,
            FMath::Sin(Angle) * Radius
        ));
    }

    // Draw filled triangle fan
    for (int32 i = 1; i < Points.Num() - 1; ++i)
    {
        FCanvasTriangleItem TriItem(
            Points[0],
            Points[i],
            Points[i + 1],
            GWhiteTexture->Resource);
        TriItem.SetColor(Color);
        Canvas->DrawItem(TriItem);
    }
}