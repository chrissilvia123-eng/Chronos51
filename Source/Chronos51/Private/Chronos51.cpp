// Copyright Chronos51 Studios 2026

#include "Chronos51.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FChronos51Module, Chronos51)

void FChronos51Module::StartupModule()
{
    // Register custom asset types, editor tools, etc.
    UE_LOG(LogTemp, Log, TEXT("Chronos51 module started"));
}

void FChronos51Module::ShutdownModule()
{
    // Cleanup
    UE_LOG(LogTemp, Log, TEXT("Chronos51 module shutdown"));
}