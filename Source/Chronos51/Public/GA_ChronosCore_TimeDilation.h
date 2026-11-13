// Chronos Core Ability - Time Dilation Bubble

#pragma once

#include "CoreMinimal.h"
#include "OrganAbilityBase.h"
#include "GA_ChronosCore_TimeDilation.generated.h"

class ATimeDilationBubble;

UCLASS(Blueprintable)
class CHRONOS51_API UGA_ChronosCore_TimeDilation : public UOrganAbilityBase
{
    GENERATED_BODY()

public:
    UGA_ChronosCore_TimeDilation();

    // Time dilation parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chronos Core")
    float TimeDilationFactor = 0.3f;  // 0.3x speed

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chronos Core")
    float BubbleRadius = 500.0f;      // 5 meters

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chronos Core")
    float Duration = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chronos Core")
    TSubclassOf<ATimeDilationBubble> BubbleClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> ActivationVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
    class USoundBase* ActivationSound;

    // Override activation
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UPROPERTY()
    ATimeDilationBubble* ActiveBubble;

    UFUNCTION()
    void OnBubbleExpired();
};