// Copyright Chronos51 Studios 2026

#include "Chronos51Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TimelineManager.h"
#include "GameInstanceSubsystem.h"

AChronos51Character::AChronos51Character()
{
    // Create components
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(false);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Camera setup
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.0f;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 8.0f; // Exponential smoothing rate
    SpringArmComponent->bEnableCameraRotationLag = true;
    SpringArmComponent->CameraRotationLagSpeed = 10.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

    // Character movement configuration
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 0.0f; // Calculated from JumpHeight/TimeToApex
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 180.0f; // Walk speed
    GetCharacterMovement()->MaxWalkSpeedCrouched = 90.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->BrakingFrictionFactor = 1.0f;

    // Capsule for mantle detection
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // Tags for GAS
    OwnedOrgans.Empty();
}

void AChronos51Character::BeginPlay()
{
    Super::BeginPlay();

    // Initialize ASC with this actor as owner
    if (AbilitySystemComponent && AttributeSet)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    // Calculate jump velocity from desired height and time to apex
    // h = 0.5 * g * t^2, v = g * t
    // g = 2 * h / t^2
    float Gravity = (2.0f * JumpHeight) / (TimeToApex * TimeToApex);
    GetCharacterMovement()->GravityScale = Gravity / 980.0f; // UE default gravity is 980 cm/s^2
    GetCharacterMovement()->JumpZVelocity = -Gravity * TimeToApex; // Negative = up in UE

    // Fall gravity multiplier handled in Tick
    BaseGravityScale = GetCharacterMovement()->GravityScale;
}

void AChronos51Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind ASC input
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Input actions will be bound via the InputConfig DataAsset in BP
        // This is where we'd bind native C++ actions if needed
    }
}

void AChronos51Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Coyote time & Jump buffer handling
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        CoyoteTimer = CoyoteTime;
    }
    else
    {
        CoyoteTimer -= DeltaTime;
    }

    if (bWantsToJump)
    {
        JumpBufferTimer = JumpBufferTime;
        bWantsToJump = false;
    }
    else
    {
        JumpBufferTimer -= DeltaTime;
    }

    // Process buffered jump
    if (JumpBufferTimer > 0.0f && CoyoteTimer > 0.0f)
    {
        Jump();
        JumpBufferTimer = 0.0f;
        CoyoteTimer = 0.0f;
    }

    // Variable jump height (cut short on release)
    if (!bJumpKeyHeld && GetCharacterMovement()->Velocity.Z > 0.0f)
    {
        FVector Vel = GetCharacterMovement()->Velocity;
        Vel.Z *= VariableJumpMultiplier;
        GetCharacterMovement()->Velocity = Vel;
    }

    // Asymmetric gravity (heavier fall)
    if (GetCharacterMovement()->Velocity.Z < 0.0f)
    {
        GetCharacterMovement()->GravityScale = BaseGravityScale * FallGravityMultiplier;
    }
    else
    {
        GetCharacterMovement()->GravityScale = BaseGravityScale;
    }

    // Mantle check
    CheckForMantle(DeltaTime);
}

UAbilitySystemComponent* AChronos51Character::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AChronos51Character::GrantOrganAbility(EOrganType OrganType)
{
    if (!OwnedOrgans.Contains(OrganType))
    {
        OwnedOrgans.Add(OrganType);
        
        // Grant corresponding GameplayAbility via ASC
        // Actual ability classes set in BP/DataAsset
        // This is a placeholder for the grant logic
    }
}

bool AChronos51Character::HasOrganAbility(EOrganType OrganType) const
{
    return OwnedOrgans.Contains(OrganType);
}

void AChronos51Character::OnPhaseShiftBegin()
{
    // Disable movement input during transition
    DisableInput(Cast<APlayerController>(GetController()));
    GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void AChronos51Character::OnPhaseShiftEnd()
{
    // Re-enable after transition
    EnableInput(Cast<APlayerController>(GetController()));
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AChronos51Character::CheckForMantle(float DeltaTime)
{
    // Simple mantle detection: trace forward and up from capsule
    // If obstructed at mid-height but clear above, mantle up
    // Full implementation would use animation root motion
    // This is a placeholder for the detection logic
}