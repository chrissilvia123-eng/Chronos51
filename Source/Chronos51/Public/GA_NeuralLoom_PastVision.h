// Neural Loom Ability - Past Vision Overlay

#pragma once

#include "CoreMinimal.h"
#include "OrganAbilityBase.h"
#include "GA_NeuralLoom_PastVision.generated.h"

UCLASS(Blueprintable)
class CHRONOS51_API UGA_NeuralLoom_PastVision : public UOrganAbilityBase
{
    GENERATED_BODY()

public:
    UGA_NeuralLoom_PastVision();

    // Past vision parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Loom")
    float ResonanceCostPerSecond = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Loom")
    TSubclassOf<class UMaterialInterface> PastVisionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Loom")
    float TransitionDuration = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> VisionVFX;

    // Toggle ability
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
    UPROPERTY()
    bool bIsVisionActive = false;

    UPROPERTY()
    class UMaterialInstanceDynamic* PastVisionMID;

    UFUNCTION()
    void OnResonanceDepleted();
};