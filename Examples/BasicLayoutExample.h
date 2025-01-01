#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealClayTypes.h"
#include "BasicLayoutExample.generated.h"

/**
 * Example actor demonstrating basic Clay UI layout
 */
UCLASS()
class ABasicLayoutExample : public AActor
{
    GENERATED_BODY()

public:
    ABasicLayoutExample();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    /** Font for UI text */
    UPROPERTY(EditAnywhere, Category = "Clay UI")
    class UFont* UIFont;

    /** Background color */
    UPROPERTY(EditAnywhere, Category = "Clay UI")
    FLinearColor BackgroundColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

    /** Button color */
    UPROPERTY(EditAnywhere, Category = "Clay UI")
    FLinearColor ButtonColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);

    /** Button hover color */
    UPROPERTY(EditAnywhere, Category = "Clay UI")
    FLinearColor ButtonHoverColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

private:
    /** Counter for click example */
    int32 ClickCount;

    /** Cached text config */
    FUnrealClayFont TextConfig;

    /** Draw main menu */
    void DrawMainMenu();

    /** Draw button section */
    void DrawButtons();

    /** Draw layout example */
    void DrawLayoutExample();
};