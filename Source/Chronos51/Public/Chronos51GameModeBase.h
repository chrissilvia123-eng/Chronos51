// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Chronos51GameModeBase.generated.h"

class UTimelineManager;
class UPuzzleManager;
class USaveSystem;

UCLASS()
class CHRONOS51_API AChronos51GameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AChronos51GameModeBase();

    virtual void BeginPlay() override;
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    // Core system accessors
    UFUNCTION(BlueprintPure, Category = "Chronos51|Systems")
    UTimelineManager* GetTimelineManager() const;

    UFUNCTION(BlueprintPure, Category = "Chronos51|Systems")
    UPuzzleManager* GetPuzzleManager() const;

    UFUNCTION(BlueprintPure, Category = "Chronos51|Systems")
    USaveSystem* GetSaveSystem() const;

    // Game state
    UFUNCTION(BlueprintCallable, Category = "Chronos51|GameState")
    void RequestAutosave();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|GameState")
    void LoadGame(int32 SlotIndex);

protected:
    UPROPERTY()
    TObjectPtr<UTimelineManager> TimelineManager;

    UPROPERTY()
    TObjectPtr<UPuzzleManager> PuzzleManager;

    UPROPERTY()
    TObjectPtr<USaveSystem> SaveSystem;
};