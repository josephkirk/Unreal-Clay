# UnrealClay

UnrealClay is an Unreal Engine plugin that integrates the Clay UI library, providing an immediate mode UI system similar to ImGui. It's designed for fast prototyping, debugging tools, and runtime UI needs.

## Features

- **Immediate Mode UI**: Write UI code similar to ImGui, with automatic layout and responsive design
- **High Performance**: Microsecond layout performance with efficient memory usage
- **Unreal Integration**: 
  - Full UMG widget support
  - Editor viewport integration
  - Blueprint support
  - PIE (Play In Editor) support
  - Asset system integration

- **Rendering Features**:
  - Efficient render target usage
  - MSAA support
  - Dynamic resizing
  - Hardware acceleration

- **UI Components**:
  - Text with font support
  - Images and textures
  - Rectangles with rounded corners 
  - Scrollable containers
  - Flexible layouts
  - Borders and decorations

## Installation

1. Clone or download this repository to your project's Plugins directory:
```bash
cd YourProject/Plugins
git clone https://github.com/josephkirk/Unreal-Clay
```

2. Regenerate your project's Visual Studio files
3. Build the project
4. Enable the UnrealClay plugin in Unreal Editor (Edit > Plugins)

## Quick Start

### Basic Usage

```cpp
// In your component or actor:
void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (auto* ClaySubsystem = UGameInstance::GetSubsystem<UUnrealClaySubsystem>())
    {
        ClaySubsystem->BeginUI();
        
        // Create a simple window
        CLAY(CLAY_LAYOUT({.padding = {16, 16}})) 
        {
            // Add text
            CLAY_TEXT(CLAY_STRING("Hello UnrealClay!"), &TextConfig);
            
            // Add a button
            CLAY(CLAY_ID("Button"), 
                CLAY_LAYOUT({.padding = {8, 8}}),
                CLAY_RECTANGLE({.color = {120, 120, 120, 255}})) 
            {
                CLAY_TEXT(CLAY_STRING("Click Me!"), &ButtonTextConfig);
            }
        }
        
        ClaySubsystem->EndUI();
    }
}
```

### Using with UMG

Add the UnrealClay widget to your UMG layout:

```cpp
// In your UUserWidget class:
UPROPERTY(meta = (BindWidget))
UUnrealClayWidget* ClayWidget;

// In your implementation:
void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (ClayWidget)
    {
        CLAY_BEGIN_UI(ClayWidget)
        {
            // Your UI code here...
        }
        CLAY_END_UI();
    }
}
```

## Documentation

For detailed documentation:
1. See the [Clay Documentation](https://github.com/nicbarker/clay) for core UI system details
2. Check the [Wiki](../../wiki) for Unreal-specific features
3. Browse example content in the Content/Examples directory

## Examples

The plugin includes several example maps demonstrating different features:

- Basic UI Layout (Examples/BasicLayout)
- Advanced Controls (Examples/AdvancedControls)
- Game HUD Example (Examples/GameHUD)
- Debug Tools (Examples/DebugTools)
- Editor Integration (Examples/EditorTools)

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Clay](https://github.com/nicbarker/clay) - The core UI library this plugin integrates
- The Unreal Engine community for feedback and support

## Contact

- Project Link: [https://github.com/josephkirk/Unreal-Clay](https://github.com/josephkirk/Unreal-Clay)
- Clay Project: [https://github.com/nicbarker/clay](https://github.com/nicbarker/clay)

## Roadmap

See the [open issues](../../issues) for a list of proposed features and known issues.