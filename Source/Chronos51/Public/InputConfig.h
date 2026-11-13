// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FChronosInputConfig
{
    GENERATED_BODY()

    // Traversal
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Move;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Look;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Jump;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Mantle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Sprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Crouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal")
    TObjectPtr<UInputAction> IA_Phase;

    // Organ abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Organ Abilities")
    TObjectPtr<UInputAction> IA_Organ_1; // Chronos Core

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Organ Abilities")
    TObjectPtr<UInputAction> IA_Organ_2; // Neural Loom

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Organ Abilities")
    TObjectPtr<UInputAction> IA_Organ_3; // Void Sail

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Organ Abilities")
    TObjectPtr<UInputAction> IA_Organ_4; // Gravity Anchor

    // Astra commands
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astra Commands")
    TObjectPtr<UInputAction> IA_Astra_Command;

    // Interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TObjectPtr<UInputAction> IA_Interact;

    // UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TObjectPtr<UInputAction> IA_Journal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TObjectPtr<UInputAction> IA_PhotoMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TObjectPtr<UInputAction> IA_Pause;

    // Mapping Contexts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_Traversal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_PuzzlePanel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_Journal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_PhotoMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_Cinematic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping Contexts")
    TObjectPtr<UInputMappingContext> IMC_AstraCommand;

    // Default bindings (for reference)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
    TMap<FName, FKey> DefaultKeyboardBindings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
    TMap<FName, FKey> DefaultGamepadBindings;
};

UCLASS(BlueprintType)
class CHRONOS51_API UInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FChronosInputConfig InputConfig;
};