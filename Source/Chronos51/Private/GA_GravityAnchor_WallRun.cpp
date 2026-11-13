// Gravity Anchor Wall Run Implementation

#include "GA_GravityAnchor_WallRun.h"
#include "Chronos51Character.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGA_GravityAnchor_WallRun::UGA_GravityAnchor_WallRun()
{
    OrganType = EOrganType::GravityAnchor;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ.GravityAnchor"));
    
    ActivationInputTag = FGameplayTag::RequestGameplayTag("Input.Organ.4");
    bIsToggleAbility = false;
    
    ResonanceCost = 0.0f;
    CooldownDuration = 0.0f;
}

void UGA_GravityAnchor_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (!Character) return;
    
    bIsWallRunning = false;
    bIsGravityShifted = false;
    WallRunTimeRemaining = 0.0f;
    GravityShiftTimeRemaining = 0.0f;
    
    Character->OnOrganActivated(OrganType);
    OnOrganActivated(Handle, ActorInfo, ActivationInfo);
}

void UGA_GravityAnchor_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        EndWallRun();
        
        if (bIsGravityShifted)
        {
            ToggleGravityShift();
        }
        
        Character->OnOrganEnded(OrganType);
    }
    
    if (WallRunTrailComponent)
    {
        WallRunTrailComponent->Deactivate();
        WallRunTrailComponent = nullptr;
    }
    
    if (GravityShiftComponent)
    {
        GravityShiftComponent->Deactivate();
        GravityShiftComponent = nullptr;
    }
    
    OnOrganEnded(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_GravityAnchor_WallRun::TickAbility(float DeltaTime)
{
    Super::TickAbility(DeltaTime);
    
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    // Handle wall run
    if (bIsWallRunning)
    {
        WallRunTimeRemaining -= DeltaTime;
        
        // Check if still valid wall run surface
        FVector CurrentWallNormal;
        if (!CheckWallRunSurface(CurrentWallNormal) || WallRunTimeRemaining <= 0.0f)
        {
            EndWallRun();
        }
        else
        {
            // Apply wall run velocity
            FVector WallRunDirection = FVector::CrossProduct(WallNormal, FVector::UpVector).GetSafeNormal();
            FVector TargetVelocity = WallRunDirection * WallRunSpeed;
            TargetVelocity.Z = Movement->Velocity.Z; // Preserve vertical
            
            Movement->Velocity = FMath::VInterpTo(Movement->Velocity, TargetVelocity, DeltaTime, 10.0f);
            Movement->GravityScale = WallRunGravityScale;
            
            // Update VFX
            if (WallRunTrailComponent)
            {
                WallRunTrailComponent->SetVariableVec3(TEXT("Direction"), WallRunDirection);
            }
        }
    }
    
    // Handle gravity shift
    if (bIsGravityShifted)
    {
        GravityShiftTimeRemaining -= DeltaTime;
        if (GravityShiftTimeRemaining <= 0.0f)
        {
            ToggleGravityShift();
        }
    }
    
    // Auto-detect wall run when falling near wall
    if (Movement->IsFalling() && !bIsWallRunning)
    {
        FVector WallNormal;
        if (CheckWallRunSurface(WallNormal))
        {
            StartWallRun(WallNormal);
        }
    }
}

bool UGA_GravityAnchor_WallRun::CheckWallRunSurface(FVector& OutWallNormal)
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return false;
    
    FVector Start = Character->GetActorLocation();
    FVector Forward = Character->GetActorForwardVector();
    FVector End = Start + Forward * WallDetectDistance;
    
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Character);
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params))
    {
        float Angle = FMath::Abs(FVector::DotProduct(HitResult.Normal, FVector::UpVector));
        float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(Angle, -1.0f, 1.0f)));
        
        if (AngleDegrees >= MinWallAngle)
        {
            OutWallNormal = HitResult.Normal;
            return true;
        }
    }
    
    return false;
}

void UGA_GravityAnchor_WallRun::StartWallRun(const FVector& WallNormalParam)
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    bIsWallRunning = true;
    WallNormal = WallNormalParam;
    WallRunTimeRemaining = WallRunMaxDuration;
    Movement->SetMovementMode(MOVE_Walking); // Custom walking on wall
    Movement->GravityScale = WallRunGravityScale;
    Movement->bOrientRotationToMovement = false;
    
    // VFX
    if (WallRunVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallRunVFX, Character->GetActorLocation());
    }
    if (WallRunTrailVFX)
    {
        WallRunTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WallRunTrailVFX, Character->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
    }
    
    // Notify character
    Character->OnWallRunStarted(WallNormal);
}

void UGA_GravityAnchor_WallRun::EndWallRun()
{
    if (!bIsWallRunning) return;
    
    bIsWallRunning = false;
    WallRunTimeRemaining = 0.0f;
    
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (Character)
    {
        UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
        if (Movement)
        {
            Movement->GravityScale = 1.0f;
            Movement->bOrientRotationToMovement = true;
            Movement->SetMovementMode(MOVE_Falling);
        }
        
        Character->OnWallRunEnded();
    }
    
    if (WallRunTrailComponent)
    {
        WallRunTrailComponent->Deactivate();
        WallRunTrailComponent = nullptr;
    }
}

void UGA_GravityAnchor_WallRun::PerformWallJump()
{
    if (!bIsWallRunning) return;
    
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    FVector JumpDirection = (WallNormal + FVector::UpVector).GetSafeNormal();
    Movement->Velocity = JumpDirection * WallJumpImpulse;
    Movement->SetMovementMode(MOVE_Falling);
    Movement->GravityScale = 1.0f;
    
    EndWallRun();
    
    // VFX
    if (WallRunVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallRunVFX, Character->GetActorLocation());
    }
}

void UGA_GravityAnchor_WallRun::ToggleGravityShift()
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    bIsGravityShifted = !bIsGravityShifted;
    
    if (bIsGravityShifted)
    {
        GravityShiftTimeRemaining = GravityShiftDuration;
        Movement->GravityScale = -0.5f; // Reverse gravity slightly
        
        // VFX
        if (GravityShiftVFX)
        {
            GravityShiftComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(GravityShiftVFX, Character->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
        }
    }
    else
    {
        GravityShiftTimeRemaining = 0.0f;
        Movement->GravityScale = 1.0f;
        
        if (GravityShiftComponent)
        {
            GravityShiftComponent->Deactivate();
            GravityShiftComponent = nullptr;
        }
    }
}