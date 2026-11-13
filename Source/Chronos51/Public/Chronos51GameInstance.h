// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chronos51GameInstance.generated.h"

UCLASS()
class CHRONOS51_API UChronos51GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UChronos51GameInstance();

    virtual void Init() override;
    virtual void Shutdown() override;

    // Quick access to subsystems
    UFUNCTION(BlueprintPure, Category = "Chronos51")
    class UTimelineManager* GetTimelineManager() const;

    UFUNCTION(BlueprintPure, Category = "Chronos51")
    class UPuzzleManager* GetPuzzleManager() const;

    UFUNCTION(BlueprintPure, Category = "Chronos51")
    class USaveSystem* GetSaveSystem() const;

    // Game state
    UPROPERTY(BlueprintReadWrite, Category = "Chronos51")
    int32 CurrentPlaythroughID = 0;

    UFUNCTION(BlueprintCallable, Category = "Chronos51")
    void StartNewPlaythrough();

    UFUNCTION(BlueprintCallable, Category = "Chronos51")
    void EndPlaythrough();
};