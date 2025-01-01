#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"

class FUnrealClayEditorModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    /** Register asset tools */
    void RegisterAssetTools();

    /** Unregister asset tools */
    void UnregisterAssetTools();

    /** Register property customizations */
    void RegisterPropertyCustomizations();

    /** Register debug tools */
    void RegisterDebugTools();

    /** Create toolbar extensions */
    void ExtendToolbar();

    /** Handle PIE state changes */
    void HandlePIEStarted(bool bIsSimulating);
    void HandlePIEEnded(bool bIsSimulating);

    /** Store created asset type actions for cleanup */
    TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

    /** Debug window widget */
    TSharedPtr<class SUnrealClayDebugWindow> DebugWindow;
};