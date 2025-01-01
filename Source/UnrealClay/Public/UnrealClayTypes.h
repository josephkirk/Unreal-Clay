#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "clay.h"
#include "UnrealClayTypes.generated.h"

// Enable Clay implementation in a single file
#ifndef CLAY_IMPLEMENTATION
#define CLAY_IMPLEMENTATION
#endif

/**
 * Configuration structure for UnrealClay render targets
 */
USTRUCT(BlueprintType)
struct UNREALCLAY_API FUnrealClayRenderConfig
{
    GENERATED_BODY()

    /** Width of the render target in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    int32 Width = 1920;

    /** Height of the render target in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay") 
    int32 Height = 1080;

    /** Whether to create linear or sRGB texture */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    bool bLinearColor = false;

    /** Whether to enable MSAA */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    bool bEnableMSAA = true;
};

/**
 * Clay font configuration wrapper
 */
USTRUCT(BlueprintType)
struct UNREALCLAY_API FUnrealClayFont 
{
    GENERATED_BODY()

    /** Font asset to use */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    class UFont* Font;

    /** Font size in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    int32 Size = 16;

    /** Letter spacing adjustment */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    float LetterSpacing = 0.0f;

    /** Font color */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    FLinearColor Color = FLinearColor::White;
};

/**
 * Clay element layout configuration wrapper
 */
USTRUCT(BlueprintType)
struct UNREALCLAY_API FUnrealClayLayout
{
    GENERATED_BODY()

    /** Padding in pixels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    FVector2D Padding = FVector2D(0, 0);

    /** Gap between child elements */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    float ChildGap = 0;

    /** Layout direction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    TEnumAsByte<EOrientation> Direction = Orient_Horizontal;

    /** Fixed width in pixels, 0 for auto */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    float FixedWidth = 0;

    /** Fixed height in pixels, 0 for auto */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    float FixedHeight = 0;

    /** Convert to Clay_LayoutConfig */
    Clay_LayoutConfig ToClay() const;
};

/**
 * Clay text configuration wrapper
 */
USTRUCT(BlueprintType)
struct UNREALCLAY_API FUnrealClayText
{
    GENERATED_BODY()

    /** Text content */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    FString Text;

    /** Font configuration */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    FUnrealClayFont Font;

    /** Text wrap mode */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    TEnumAsByte<ETextWrappingPolicy> WrapMode = ETextWrappingPolicy::DefaultWrapping;

    /** Convert to Clay text element config */
    Clay_TextElementConfig ToClay() const;
};

/**
 * Clay image configuration wrapper
 */
USTRUCT(BlueprintType)
struct UNREALCLAY_API FUnrealClayImage
{
    GENERATED_BODY()

    /** Image texture */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    class UTexture2D* Texture;

    /** Tint color */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealClay")
    FLinearColor Color = FLinearColor::White;

    /** Convert to Clay image config */
    Clay_ImageElementConfig ToClay() const;
};

/**
 * Conversion helpers between Unreal and Clay types
 */
namespace UnrealClayHelpers
{
    /** Convert FLinearColor to Clay_Color */
    inline Clay_Color ToClayColor(const FLinearColor& Color)
    {
        return Clay_Color{
            Color.R * 255.0f,
            Color.G * 255.0f, 
            Color.B * 255.0f,
            Color.A * 255.0f
        };
    }

    /** Convert Clay_Color to FLinearColor */
    inline FLinearColor ToUnrealColor(const Clay_Color& Color) 
    {
        return FLinearColor(
            Color.r / 255.0f,
            Color.g / 255.0f,
            Color.b / 255.0f,
            Color.a / 255.0f
        );
    }

    /** Convert FVector2D to Clay_Vector2 */
    inline Clay_Vector2 ToClayVector(const FVector2D& Vector)
    {
        return Clay_Vector2{Vector.X, Vector.Y};
    }

    /** Convert Clay_Vector2 to FVector2D */
    inline FVector2D ToUnrealVector(const Clay_Vector2& Vector)
    {
        return FVector2D(Vector.x, Vector.y);
    }

    /** Convert FString to Clay_String */
    inline Clay_String ToClayString(const FString& String)
    {
        return Clay_String{
            String.Len(),
            TCHAR_TO_UTF8(*String)
        };
    }
}