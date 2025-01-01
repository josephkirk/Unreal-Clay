#include "BasicLayoutExample.h"
#include "UnrealClaySubsystem.h"
#include "Engine/Font.h"

ABasicLayoutExample::ABasicLayoutExample()
    : ClickCount(0)
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABasicLayoutExample::BeginPlay()
{
    Super::BeginPlay();

    // Setup text config
    TextConfig.Font = UIFont;
    TextConfig.Size = 16;
    TextConfig.Color = FLinearColor::White;
}

void ABasicLayoutExample::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (auto* ClaySubsystem = GetGameInstance()->GetSubsystem<UUnrealClaySubsystem>())
    {
        ClaySubsystem->BeginUI();
        
        // Main container
        CLAY(CLAY_LAYOUT({ 
            .sizing = { 
                .width = CLAY_SIZING_FIXED(800),
                .height = CLAY_SIZING_FIXED(600)
            },
            .padding = { 16, 16 },
            .childGap = 16,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        }), CLAY_RECTANGLE({ .color = UnrealClayHelpers::ToClayColor(BackgroundColor) })) 
        {
            DrawMainMenu();
            DrawButtons();
            DrawLayoutExample();
        }

        ClaySubsystem->EndUI();
    }
}

void ABasicLayoutExample::DrawMainMenu()
{
    // Header
    CLAY(CLAY_LAYOUT({ .padding = { 8, 8 } })) 
    {
        CLAY_TEXT(CLAY_STRING("UnrealClay Basic Example"), TextConfig);
    }

    // Horizontal menu
    CLAY(CLAY_LAYOUT({ 
        .padding = { 8, 8 },
        .childGap = 8,
        .layoutDirection = CLAY_LEFT_TO_RIGHT
    })) 
    {
        // Menu items
        const char* MenuItems[] = { "File", "Edit", "View", "Help" };
        for (const char* Item : MenuItems)
        {
            CLAY(CLAY_LAYOUT({ .padding = { 8, 4 } })) 
            {
                CLAY_TEXT(CLAY_STRING(Item), TextConfig);
            }
        }
    }
}

void ABasicLayoutExample::DrawButtons()
{
    // Button row
    CLAY(CLAY_LAYOUT({ 
        .padding = { 8, 8 },
        .childGap = 8,
        .layoutDirection = CLAY_LEFT_TO_RIGHT
    })) 
    {
        // Clickable button
        CLAY(
            CLAY_ID("ClickButton"),
            CLAY_LAYOUT({ .padding = { 16, 8 } }),
            CLAY_RECTANGLE({ 
                .color = Clay_Hovered() ? 
                    UnrealClayHelpers::ToClayColor(ButtonHoverColor) : 
                    UnrealClayHelpers::ToClayColor(ButtonColor),
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            })
        ) 
        {
            // Button text shows click count
            char ButtonText[32];
            snprintf(ButtonText, sizeof(ButtonText), "Clicked %d times", ClickCount);
            CLAY_TEXT(CLAY_STRING(ButtonText), TextConfig);
        }

        // Check if button was clicked
        if (Clay_PointerOver(Clay_GetElementId("ClickButton")))
        {
            Clay_PointerData PointerData;
            if (PointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME)
            {
                ClickCount++;
            }
        }

        // Reset button
        CLAY(
            CLAY_ID("ResetButton"),
            CLAY_LAYOUT({ .padding = { 16, 8 } }),
            CLAY_RECTANGLE({ 
                .color = Clay_Hovered() ? 
                    UnrealClayHelpers::ToClayColor(ButtonHoverColor) : 
                    UnrealClayHelpers::ToClayColor(ButtonColor),
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            })
        ) 
        {
            CLAY_TEXT(CLAY_STRING("Reset"), TextConfig);
        }

        // Check if reset button was clicked
        if (Clay_PointerOver(Clay_GetElementId("ResetButton")))
        {
            Clay_PointerData PointerData;
            if (PointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME)
            {
                ClickCount = 0;
            }
        }
    }
}

void ABasicLayoutExample::DrawLayoutExample()
{
    // Grid layout example
    CLAY(CLAY_LAYOUT({ 
        .padding = { 16, 16 },
        .childGap = 8,
        .layoutDirection = CLAY_TOP_TO_BOTTOM
    }), CLAY_RECTANGLE({ 
        .color = UnrealClayHelpers::ToClayColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f)),
        .cornerRadius = CLAY_CORNER_RADIUS(8)
    })) 
    {
        // Title
        CLAY_TEXT(CLAY_STRING("Layout Example"), TextConfig);

        // Row 1
        CLAY(CLAY_LAYOUT({ 
            .childGap = 8,
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        })) 
        {
            for (int i = 0; i < 3; i++)
            {
                CLAY(CLAY_LAYOUT({ 
                    .sizing = { .width = CLAY_SIZING_GROW() },
                    .padding = { 8, 8 }
                }), CLAY_RECTANGLE({ 
                    .color = UnrealClayHelpers::ToClayColor(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f)),
                    .cornerRadius = CLAY_CORNER_RADIUS(4)
                })) 
                {
                    char Text[32];
                    snprintf(Text, sizeof(Text), "Item %d", i + 1);
                    CLAY_TEXT(CLAY_STRING(Text), TextConfig);
                }
            }
        }

        // Row 2 with different sizes
        CLAY(CLAY_LAYOUT({ 
            .childGap = 8,
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        })) 
        {
            // 25% width
            CLAY(CLAY_LAYOUT({ 
                .sizing = { .width = CLAY_SIZING_PERCENT(0.25f) },
                .padding = { 8, 8 }
            }), CLAY_RECTANGLE({ 
                .color = UnrealClayHelpers::ToClayColor(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f)),
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            })) 
            {
                CLAY_TEXT(CLAY_STRING("25%"), TextConfig);
            }

            // 75% width
            CLAY(CLAY_LAYOUT({ 
                .sizing = { .width = CLAY_SIZING_PERCENT(0.75f) },
                .padding = { 8, 8 }
            }), CLAY_RECTANGLE({ 
                .color = UnrealClayHelpers::ToClayColor(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f)),
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            })) 
            {
                CLAY_TEXT(CLAY_STRING("75%"), TextConfig);
            }
        }

        // Scroll container example
        CLAY(CLAY_LAYOUT({ 
            .sizing = { 
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_FIXED(100)
            }
        }), CLAY_SCROLL({ .vertical = true })) 
        {
            // Content
            CLAY(CLAY_LAYOUT({ 
                .padding = { 8, 8 },
                .childGap = 8,
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            })) 
            {
                for (int i = 0; i < 10; i++)
                {
                    char Text[32];
                    snprintf(Text, sizeof(Text), "Scroll Item %d", i + 1);
                    CLAY_TEXT(CLAY_STRING(Text), TextConfig);
                }
            }
        }
    }
}