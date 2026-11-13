// Void Sail Double Jump & Glide Implementation

#include "GA_VoidSail_DoubleJumpGlide.h"
#include "Chronos51Character.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_VoidSail_DoubleJumpGlide::UGA_VoidSail_DoubleJumpGlide()
{
    OrganType = EOrganType::VoidSail;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ.VoidSail"));
    
    ActivationInputTag = FGameplayTag::RequestGameplayTag("Input.Organ.3");
    bIsToggleAbility = false;
    
    ResonanceCost = 0.0f;
    CooldownDuration = 0.0f;
}

void UGA_VoidSail_DoubleJumpGlide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (!Character) return;
    
    AirJumpsUsed = 0;
    bIsGliding = false;
    
    // Listen for landed event
    Character->OnCharacterLanded.AddDynamic(this, &UGA_VoidSail_DoubleJumpGlide::OnLanded);
    
    // Enable glide movement mode
    if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
    {
        Movement->SetMovementMode(MOVE_Falling);
    }
    
    Character->OnOrganActivated(OrganType);
    OnOrganActivated(Handle, ActorInfo, ActivationInfo);
}

void UGA_VoidSail_DoubleJumpGlide::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        Character->OnCharacterLanded.RemoveDynamic(this, &UGA_VoidSail_DoubleJumpGlide::OnLanded);
        
        // Stop gliding
        bIsGliding = false;
        if (GlideTrailComponent)
        {
            GlideTrailComponent->Deactivate();
            GlideTrailComponent = nullptr;
        }
        
        Character->OnOrganEnded(OrganType);
    }
    
    OnOrganEnded(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_VoidSail_DoubleJumpGlide::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (!Character) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    if (Movement->IsFalling())
    {
        if (AirJumpsUsed < MaxAirJumps)
        {
            // Double jump
            FVector JumpVelocity = FVector(0, 0, DoubleJumpVelocity);
            Movement->Velocity.Z = DoubleJumpVelocity;
            AirJumpsUsed++;
            
            // VFX
            if (DoubleJumpVFX)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DoubleJumpVFX, Character->GetActorLocation());
            }
        }
        else if (!bIsGliding)
        {
            // Start gliding
            bIsGliding = true;
            Movement->SetMovementMode(MOVE_Falling);
            Movement->GravityScale = GlideFallSpeed / 980.0f; // Normalize to UE gravity
            
            // VFX
            if (GlideVFX)
            {
                UNiagaraFunctionLibrary::SpawnSystemAttached(GlideVFX, Character->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
            }
            if (GlideTrailVFX)
            {
                GlideTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(GlideTrailVFX, Character->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
            }
        }
    }
}

void UGA_VoidSail_DoubleJumpGlide::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (!Character) return;
    
    if (bIsGliding)
    {
        bIsGliding = false;
        
        UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
        if (Movement)
        {
            Movement->GravityScale = 1.0f;
        }
        
        if (GlideTrailComponent)
        {
            GlideTrailComponent->Deactivate();
            GlideTrailComponent = nullptr;
        }
    }
}

void UGA_VoidSail_DoubleJumpGlide::OnLanded(const FHitResult& Hit)
{
    AirJumpsUsed = 0;
    bIsGliding = false;
    
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (Character)
    {
        UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
        if (Movement)
        {
            Movement->GravityScale = 1.0f;
        }
        
        if (GlideTrailComponent)
        {
            GlideTrailComponent->Deactivate();
            GlideTrailComponent = nullptr;
        }
    }
}

void UGA_VoidSail_DoubleJumpGlide::UpdateGlide(float DeltaTime)
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character || !bIsGliding) return;
    
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;
    
    // Apply glide forward velocity based on input
    FVector InputVector = Character->GetLastMovementInputVector();
    if (InputVector.SizeSquared() > 0.0f)
    {
        FVector GlideVelocity = InputVector.GetSafeNormal() * GlideForwardSpeed;
        GlideVelocity.Z = Movement->Velocity.Z; // Preserve vertical velocity
        Movement->Velocity = GlideVelocity;
    }
}