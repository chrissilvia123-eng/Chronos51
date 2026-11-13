// Organ Abilities - GameplayAbility C++ Base Classes
// Each organ grants a unique traversal ability via GAS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OrganAbilityBase.generated.h"

UENUM(BlueprintType)
enum class EOrganType : uint8
{
    None        UMETA(DisplayName = "None"),
    ChronosCore UMETA(DisplayName = "Chronos Core - Time Dilation"),
    NeuralLoom  UMETA(DisplayName = "Neural Loom - Past Vision"),
    VoidSail    UMETA(DisplayName = "Void Sail - Double Jump/Glide"),
    GravityAnchor UMETA(DisplayName = "Gravity Anchor - Wall Run"),
    AstraCore   UMETA(DisplayName = "Astra Core - Synthesis")
};

UCLASS(Abstract, Blueprintable)
class CHRONOS51_API UOrganAbilityBase : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UOrganAbilityBase();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Organ")
    EOrganType OrganType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Organ")
    FGameplayTagContainer OrganTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Organ")
    FGameplayTagContainer BlockedTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
    TSubclassOf<UGameplayEffect> ResonanceCostEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
    float ResonanceCost = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
    float CooldownDuration = 0.0f;

    // Organ-specific activation logic
    UFUNCTION(BlueprintNativeEvent, Category = "Organ")
    void OnOrganActivated(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

    UFUNCTION(BlueprintNativeEvent, Category = "Organ")
    void OnOrganEnded(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicated, bool bWasCancelled);

    // Check if player has required organ
    UFUNCTION(BlueprintPure, Category = "Organ")
    static bool HasOrgan(const UAbilitySystemComponent* ASC, EOrganType RequiredOrgan);

    // Get organ tag for ASC queries
    UFUNCTION(BlueprintPure, Category = "Organ")
    static FGameplayTag GetOrganTag(EOrganType OrganType);
};