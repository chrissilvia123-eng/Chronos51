// Void Sail Ability - Double Jump & Glide

#pragma once

#include "CoreMinimal.h"
#include "OrganAbilityBase.h"
#include "GA_VoidSail_DoubleJumpGlide.generated.h"

UCLASS(Blueprintable)
class CHRONOS51_API UGA_VoidSail_DoubleJumpGlide : public UOrganAbilityBase
{
    GENERATED_BODY()

public:
    UGA_VoidSail_DoubleJumpGlide();

    // Double jump parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Void Sail")
    float DoubleJumpVelocity = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Void Sail")
    int32 MaxAirJumps = 1;

    // Glide parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Void Sail")
    float GlideFallSpeed = 150.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Void Sail")
    float GlideForwardSpeed = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Void Sail")
    float GlideTurnRate = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> DoubleJumpVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> GlideVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> GlideTrailVFX;

    // Input binding
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    FGameplayTag InputTag_Jump = FGameplayTag::RequestGameplayTag("Input.Jump");

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
    virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
    UPROPERTY()
    int32 AirJumpsUsed = 0;

    UPROPERTY()
    bool bIsGliding = false;

    UPROPERTY()
    class UNiagaraComponent* GlideTrailComponent;

    UFUNCTION()
    void OnLanded(const FHitResult& Hit);

    UFUNCTION()
    void UpdateGlide(float DeltaTime);
};