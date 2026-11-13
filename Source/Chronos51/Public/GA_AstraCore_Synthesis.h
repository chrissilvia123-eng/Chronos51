// Astra Core Ability - Final Synthesis (Cinematic)

#pragma once

#include "CoreMinimal.h"
#include "OrganAbilityBase.h"
#include "GA_AstraCore_Synthesis.generated.h"

UCLASS(Blueprintable)
class CHRONOS51_API UGA_AstraCore_Synthesis : public UOrganAbilityBase
{
    GENERATED_BODY()

public:
    UGA_AstraCore_Synthesis();

    // Synthesis parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Astra Core")
    float SynthesisDuration = 60.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Astra Core")
    int32 RequiredTrustLevel = 8;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Astra Core")
    int32 RequiredFragments = 12;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Astra Core")
    TArray<EOrganType> RequiredOrgans = {EOrganType::ChronosCore, EOrganType::NeuralLoom, EOrganType::VoidSail, EOrganType::GravityAnchor};

    // Cinematic
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cinematic")
    class ULevelSequence* SynthesisSequence;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cinematic")
    TArray<class ULevelSequence*> EndingSequences; // 4 endings

    // VFX
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> SynthesisVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> OrganMergeVFX[4];

    // Ending determination
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ending")
    FGameplayTag EndingTag_SavedCorwin = FGameplayTag::RequestGameplayTag("Ending.SavedCorwin");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ending")
    FGameplayTag EndingTag_FreedAstra = FGameplayTag::RequestGameplayTag("Ending.FreedAstra");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ending")
    FGameplayTag EndingTag_SacrificedSelf = FGameplayTag::RequestGameplayTag("Ending.SacrificedSelf");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ending")
    FGameplayTag EndingTag_TrustedCorwin = FGameplayTag::RequestGameplayTag("Ending.TrustedCorwin");

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
    UPROPERTY()
    int32 CurrentSynthesisStage = 0;

    UPROPERTY()
    class ALevelSequenceActor* SequenceActor;

    UFUNCTION()
    void OnSynthesisStageComplete(int32 Stage);

    UFUNCTION()
    void DetermineEnding();

    UFUNCTION()
    void PlayEndingSequence(int32 EndingIndex);

    UFUNCTION()
    bool CheckRequirements() const;
};