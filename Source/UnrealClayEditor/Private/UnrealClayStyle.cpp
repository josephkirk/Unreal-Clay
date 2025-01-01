#include "UnrealClayStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define ICON_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FUnrealClayEditorStyle::StyleInstance = nullptr;

void FUnrealClayEditorStyle::Initialize()
{
    if (!StyleInstance.IsValid())
    {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FUnrealClayEditorStyle::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

void FUnrealClayEditorStyle::ReloadTextures()
{
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FUnrealClayEditorStyle::Get()
{
    return *StyleInstance;
}

FName FUnrealClayEditorStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT("UnrealClayEditorStyle"));
    return StyleSetName;
}

TSharedRef<FSlateStyleSet> FUnrealClayEditorStyle::Create()
{
    TSharedRef<FSlateStyleSet> StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

    // Load plugin content directory
    StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("UnrealClay"))->GetBaseDir() / TEXT("Resources"));

    // Register brushes
    StyleSet->Set("UnrealClay.DebuggerIcon", new ICON_BRUSH("Icons/ClayDebugger_40x", FVector2D(40.0f, 40.0f)));
    StyleSet->Set("UnrealClay.DebuggerIcon.Small", new ICON_BRUSH("Icons/ClayDebugger_16x", FVector2D(16.0f, 16.0f)));

    return StyleSet;
}

#undef ICON_BRUSH