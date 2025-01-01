#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnrealClayTypes.h"
#include "UnrealClayRenderTarget.h"
#include "UnrealClaySubsystem.generated.h"

/** Delegate for Clay events like clicks and hover */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClayEvent, const FString&, ElementId);

/**
 * Game instance subsystem that manages Clay UI system
 */
UCLASS()
class UNREALCLAY_API UUnrealClaySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /** Constructor */
    UUnrealClaySubsystem();

    //~ Begin USubsystem Interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    //~ End USubsystem Interface

    /** Begin new UI frame - must be called before any Clay UI commands */
    UFUNCTION(BlueprintCallable, Category = "UnrealClay")
    void BeginUI();

    /** End UI frame and render - must be called after all Clay UI commands */
    UFUNCTION(BlueprintCallable, Category = "UnrealClay")
    void EndUI();

    /** Set default font to use for text */
    UFUNCTION(BlueprintCallable, Category = "UnrealClay")
    void SetDefaultFont(const FUnrealClayFont& InFont);

    /** Update input state for Clay */
    void UpdateInput(const FVector2D& MousePosition, bool bMouseDown, const FVector2D& ScrollDelta);

    /** Get current render target */
    UFUNCTION(BlueprintPure, Category = "UnrealClay")
    UUnrealClayRenderTarget* GetRenderTarget() const { return RenderTarget; }

    /** Set render target config */
    UFUNCTION(BlueprintCallable, Category = "UnrealClay")
    void SetRenderConfig(const FUnrealClayRenderConfig& Config);

    /** Event when element is clicked */
    UPROPERTY(BlueprintAssignable, Category = "UnrealClay")
    FOnClayEvent OnElementClicked;

    /** Event when element is hovered */
    UPROPERTY(BlueprintAssignable, Category = "UnrealClay")
    FOnClayEvent OnElementHovered;

protected:
    /** Initialize Clay memory and systems */
    void InitializeClay();

    /** Clean up Clay memory and systems */
    void CleanupClay();

    /** Measure text callback for Clay */
    static Clay_Dimensions MeasureText(Clay_String* Text, Clay_TextElementConfig* Config);

    /** Convert Clay render commands to Unreal draw commands */
    void ProcessRenderCommands(const Clay_RenderCommandArray& Commands);

private:
    /** Clay arena memory */
    void* ClayMemory;

    /** Current render target */
    UPROPERTY()
    UUnrealClayRenderTarget* RenderTarget;

    /** Default font settings */
    FUnrealClayFont DefaultFont;

    /** Last mouse position */
    FVector2D LastMousePosition;

    /** Last frame time */
    float LastFrameTime;

    /** Whether Clay is initialized */
    bool bIsInitialized;
};