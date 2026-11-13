// Gravity Anchor Ability - Wall Run & Gravity Manipulation

#pragma once

#include "CoreMinimal.h"
#include "OrganAbilityBase.h"
#include "GA_GravityAnchor_WallRun.generated.h"

UCLASS(Blueprintable)
class CHRONOS51_API UGA_GravityAnchor_WallRun : public UOrganAbilityBase
{
    GENERATED_BODY()

public:
    UGA_GravityAnchor_WallRun();

    // Wall run parameters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float WallRunSpeed = 500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float WallRunMaxDuration = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float WallRunGravityScale = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float WallJumpImpulse = 800.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float WallDetectDistance = 80.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float MinWallAngle = 80.0f; // Degrees from vertical

    // Gravity shift (toggle)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float GravityShiftDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gravity Anchor")
    float GravityShiftCooldown = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> WallRunVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> GravityShiftVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    TSubclassOf<class UNiagaraSystem> WallRunTrailVFX;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual void TickAbility(float DeltaTime) override;

protected:
    UPROPERTY()
    bool bIsWallRunning = false;

    UPROPERTY()
    bool bIsGravityShifted = false;

    UPROPERTY()
    FVector WallNormal;

    UPROPERTY()
    class UNiagaraComponent* WallRunTrailComponent;

    UPROPERTY()
    class UNiagaraComponent* GravityShiftComponent;

    UPROPERTY()
    float WallRunTimeRemaining = 0.0f;

    UPROPERTY()
    float GravityShiftTimeRemaining = 0.0f;

    UFUNCTION()
    bool CheckWallRunSurface(FVector& OutWallNormal);

    UFUNCTION()
    void StartWallRun(const FVector& WallNormal);

    UFUNCTION()
    void EndWallRun();

    UFUNCTION()
    void PerformWallJump();

    UFUNCTION()
    void ToggleGravityShift();
};