#include "UnrealClayTypes.h"
#include "Engine/Font.h"
#include "Engine/Texture2D.h"
#include "UObject/PropertyPortFlags.h"

Clay_LayoutConfig FUnrealClayLayout::ToClay() const
{
    Clay_LayoutConfig Config = CLAY_LAYOUT_DEFAULT;
    
    // Set padding
    Config.padding = Clay_Padding{
        static_cast<uint16>(Padding.X),
        static_cast<uint16>(Padding.Y)
    };
    
    // Set child gap
    Config.childGap = static_cast<uint16>(ChildGap);
    
    // Set layout direction
    Config.layoutDirection = Direction == Orient_Horizontal ? 
        CLAY_LEFT_TO_RIGHT : CLAY_TOP_TO_BOTTOM;
    
    // Set sizing
    if (FixedWidth > 0)
    {
        Config.sizing.width = CLAY_SIZING_FIXED(FixedWidth);
    }
    else 
    {
        Config.sizing.width = CLAY_SIZING_GROW();
    }
    
    if (FixedHeight > 0) 
    {
        Config.sizing.height = CLAY_SIZING_FIXED(FixedHeight);
    }
    else
    {
        Config.sizing.height = CLAY_SIZING_GROW();
    }
    
    return Config;
}

Clay_TextElementConfig FUnrealClayText::ToClay() const
{
    Clay_TextElementConfig Config = {};
    
    // Set text color
    Config.textColor = UnrealClayHelpers::ToClayColor(Font.Color);
    
    // Set font ID - should match ID registered with font system
    if (Font.Font)
    {
        Config.fontId = Font.Font->GetUniqueID();
    }
    
    // Set font size
    Config.fontSize = Font.Size;
    
    // Set letter spacing
    Config.letterSpacing = static_cast<uint16>(Font.LetterSpacing);
    
    // Set line height - using font size as default
    Config.lineHeight = Font.Size;
    
    // Set wrap mode
    switch (WrapMode)
    {
        case ETextWrappingPolicy::DefaultWrapping:
            Config.wrapMode = CLAY_TEXT_WRAP_WORDS;
            break;
        case ETextWrappingPolicy::AllowPerCharacterWrapping:
            Config.wrapMode = CLAY_TEXT_WRAP_NONE;
            break;
        default:
            Config.wrapMode = CLAY_TEXT_WRAP_WORDS;
            break;
    }
    
    return Config;
}

Clay_ImageElementConfig FUnrealClayImage::ToClay() const
{
    Clay_ImageElementConfig Config = {};
    
    if (Texture)
    {
        // Store texture pointer for renderer to use
        Config.imageData = Texture;
        
        // Set source dimensions from texture
        Config.sourceDimensions = Clay_Dimensions{
            static_cast<float>(Texture->GetSizeX()),
            static_cast<float>(Texture->GetSizeY())
        };
    }
    
    return Config;
}

#if WITH_EDITOR

bool FUnrealClayFont::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    const FName PropertyName = PropertyChangedEvent.GetPropertyName();
    bool bHandled = false;
    
    // Update letter spacing range
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayFont, LetterSpacing))
    {
        LetterSpacing = FMath::Clamp(LetterSpacing, -10.0f, 10.0f);
        bHandled = true;
    }
    
    // Update font size range
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayFont, Size))
    {
        Size = FMath::Clamp(Size, 1, 256);
        bHandled = true;
    }
    
    return bHandled;
}

void FUnrealClayFont::PostSerialize(const FArchive& Ar)
{
    if (Ar.IsLoading())
    {
        // Validate ranges when loading
        LetterSpacing = FMath::Clamp(LetterSpacing, -10.0f, 10.0f);
        Size = FMath::Clamp(Size, 1, 256);
    }
}

bool FUnrealClayLayout::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    const FName PropertyName = PropertyChangedEvent.GetPropertyName();
    bool bHandled = false;
    
    // Validate padding ranges
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayLayout, Padding))
    {
        Padding.X = FMath::Max(0.0f, Padding.X);
        Padding.Y = FMath::Max(0.0f, Padding.Y);
        bHandled = true;
    }
    
    // Validate child gap
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayLayout, ChildGap))
    {
        ChildGap = FMath::Max(0.0f, ChildGap);
        bHandled = true;
    }
    
    // Validate fixed dimensions
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayLayout, FixedWidth))
    {
        FixedWidth = FMath::Max(0.0f, FixedWidth);
        bHandled = true;
    }
    
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayLayout, FixedHeight))
    {
        FixedHeight = FMath::Max(0.0f, FixedHeight);
        bHandled = true;
    }
    
    return bHandled;
}

bool FUnrealClayRenderConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    const FName PropertyName = PropertyChangedEvent.GetPropertyName();
    bool bHandled = false;
    
    // Validate dimensions
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayRenderConfig, Width))
    {
        Width = FMath::Clamp(Width, 1, 16384);
        bHandled = true;
    }
    
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FUnrealClayRenderConfig, Height))
    {
        Height = FMath::Clamp(Height, 1, 16384);
        bHandled = true;
    }
    
    return bHandled;
}

#endif // WITH_EDITOR