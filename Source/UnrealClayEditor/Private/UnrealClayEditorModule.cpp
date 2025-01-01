#include "UnrealClayEditorModule.h"
#include "UnrealClayStyle.h"
#include "PropertyEditorModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Docking/SDockTab.h"
#include "UnrealClayDebugWindow.h"
#include "ISettingsModule.h"
#include "UnrealClaySettings.h"

#define LOCTEXT_NAMESPACE "UnrealClayEditorModule"

void FUnrealClayEditorModule::StartupModule()
{
    // Register slate style
    FUnrealClayEditorStyle::Initialize();
    FUnrealClayEditorStyle::ReloadTextures();

    // Register property customizations
    RegisterPropertyCustomizations();

    // Register asset tools
    RegisterAssetTools();

    // Register debug tools
    RegisterDebugTools();

    // Extend toolbar
    ExtendToolbar();

    // Register settings
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->RegisterSettings("Project", "Plugins", "UnrealClay",
            LOCTEXT("UnrealClaySettingsName", "UnrealClay"),
            LOCTEXT("UnrealClaySettingsDescription", "Configure UnrealClay plugin settings"),
            GetMutableDefault<UUnrealClaySettings>()
        );
    }

    // Register PIE listeners
    FEditorDelegates::PreBeginPIE.AddRaw(this, &FUnrealClayEditorModule::HandlePIEStarted);
    FEditorDelegates::EndPIE.AddRaw(this, &FUnrealClayEditorModule::HandlePIEEnded);
}

void FUnrealClayEditorModule::ShutdownModule()
{
    // Unregister PIE listeners
    FEditorDelegates::PreBeginPIE.RemoveAll(this);
    FEditorDelegates::EndPIE.RemoveAll(this);

    // Unregister settings
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "UnrealClay");
    }

    // Cleanup style
    FUnrealClayEditorStyle::Shutdown();

    // Unregister property customizations
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        // Unregister all custom property type layouts
    }

    // Unregister asset tools
    UnregisterAssetTools();
}

void FUnrealClayEditorModule::RegisterAssetTools()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    // Register asset type actions here
}

void FUnrealClayEditorModule::UnregisterAssetTools()
{
    CreatedAssetTypeActions.Empty();
}

void FUnrealClayEditorModule::RegisterPropertyCustomizations()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        // Register custom property type layouts here
    }
}

void FUnrealClayEditorModule::RegisterDebugTools()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        "UnrealClayDebugger",
        FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args)
        {
            return SNew(SDockTab)
                .TabRole(ETabRole::NomadTab)
                [
                    SAssignNew(DebugWindow, SUnrealClayDebugWindow)
                ];
        }))
        .SetDisplayName(LOCTEXT("UnrealClayDebuggerTab", "Clay UI Debugger"))
        .SetTooltipText(LOCTEXT("UnrealClayDebuggerTooltip", "Open the Clay UI debugger window"))
        .SetIcon(FSlateIcon(FUnrealClayEditorStyle::GetStyleSetName(), "UnrealClay.DebuggerIcon"));
}

void FUnrealClayEditorModule::ExtendToolbar()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

    ToolbarExtender->AddToolBarExtension(
        "Settings",
        EExtensionHook::After,
        nullptr,
        FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& Builder)
        {
            Builder.AddToolBarButton(
                FUIAction(
                    FExecuteAction::CreateLambda([this]()
                    {
                        FGlobalTabmanager::Get()->TryInvokeTab(FTabId("UnrealClayDebugger"));
                    })
                ),
                NAME_None,
                LOCTEXT("ClayUIDebugger", "Clay UI Debugger"),
                LOCTEXT("ClayUIDebuggerTooltip", "Open Clay UI debugger window"),
                FSlateIcon(FUnrealClayEditorStyle::GetStyleSetName(), "UnrealClay.DebuggerIcon")
            );
        })
    );

    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FUnrealClayEditorModule::HandlePIEStarted(bool bIsSimulating)
{
    // Handle PIE start
    if (DebugWindow.IsValid())
    {
        DebugWindow->OnPIEStarted(bIsSimulating);
    }
}

void FUnrealClayEditorModule::HandlePIEEnded(bool bIsSimulating)
{
    // Handle PIE end
    if (DebugWindow.IsValid())
    {
        DebugWindow->OnPIEEnded(bIsSimulating);
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealClayEditorModule, UnrealClayEditor)