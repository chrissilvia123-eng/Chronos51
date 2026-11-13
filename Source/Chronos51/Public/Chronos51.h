// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FChronos51Module : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};