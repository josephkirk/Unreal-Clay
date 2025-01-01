#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UNREALCLAY_API FUnrealClayModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    /** Handle to the plugin's shader library */
    void* ClayShaderLibraryHandle;

    /** Register shader library */
    void RegisterShaders();

    /** Unregister shader library */
    void UnregisterShaders();
};