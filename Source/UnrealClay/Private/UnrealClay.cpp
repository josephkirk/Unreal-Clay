#include "UnrealClay.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FUnrealClayModule"

void FUnrealClayModule::StartupModule()
{
    // Register shaders
    RegisterShaders();

    // Log startup
    UE_LOG(LogTemp, Log, TEXT("UnrealClay: Module started"));
}

void FUnrealClayModule::ShutdownModule()
{
    // Unregister shaders
    UnregisterShaders();
}

void FUnrealClayModule::RegisterShaders()
{
    // Get shader directory
    FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("UnrealClay"))->GetBaseDir(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping(TEXT("/Plugin/UnrealClay"), PluginShaderDir);
}

void FUnrealClayModule::UnregisterShaders()
{
    // Cleanup shader mappings
    ResetAllShaderSourceDirectoryMappings();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUnrealClayModule, UnrealClay)