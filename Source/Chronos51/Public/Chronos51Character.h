// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Chronos51Character.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UCameraComponent;
class USpringArmComponent;
class UTimelineManager;

UENUM(BlueprintType)
enum class EOrganType : uint8
{
    None,
    ChronosCore,    // Time Dilation Bubble
    NeuralLoom,     // Past Vision Overlay
    VoidSail,       // Double Jump + Glide
    GravityAnchor,  // Wall Run / Ceiling Hang
    AstraCore       // Final Synthesis
};

UCLASS()
class CHRONOS51_API AChronos51Character : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AChronos51Character();

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // Movement configuration
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Movement")
    void SetMovementMode(EMovementMode NewMovementMode);

    // Organ abilities
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Abilities")
    void GrantOrganAbility(EOrganType OrganType);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Abilities")
    bool HasOrganAbility(EOrganType OrganType) const;

    // Camera
    UFUNCTION(BlueprintPure, Category = "Chronos51|Camera")
    UCameraComponent* GetCameraComponent() const { return CameraComponent; }

    UFUNCTION(BlueprintPure, Category = "Chronos51|Camera")
    USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

    // Phase Shift integration
    UFUNCTION(BlueprintCallable, Category = "Chronos51|PhaseShift")
    void OnPhaseShiftBegin();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|PhaseShift")
    void OnPhaseShiftEnd();

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chronos51|Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chronos51|Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAttributeSet> AttributeSet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chronos51|Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chronos51|Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    // Organ tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chronos51|Organs")
    TArray<EOrganType> OwnedOrgans;

    // Movement feel parameters (tuned in BP)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float JumpHeight = 220.0f; // cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float TimeToApex = 0.35f; // seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float FallGravityMultiplier = 1.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float CoyoteTime = 0.1f; // seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float JumpBufferTime = 0.12f; // seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float VariableJumpMultiplier = 0.45f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|MovementFeel")
    float MantleHeight = 150.0f; // cm

    // Internal state
    float CoyoteTimer = 0.0f;
    float JumpBufferTimer = 0.0f;
    bool bWantsToJump = false;
    bool bJumpKeyHeld = false;

    // Mantle detection
    UFUNCTION()
    void CheckForMantle(float DeltaTime);
};