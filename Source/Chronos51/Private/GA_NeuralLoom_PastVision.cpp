// Neural Loom Past Vision Implementation

#include "GA_NeuralLoom_PastVision.h"
#include "Chronos51Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Camera/CameraComponent.h"

UGA_NeuralLoom_PastVision::UGA_NeuralLoom_PastVision()
{
    OrganType = EOrganType::NeuralLoom;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ.NeuralLoom"));
    
    ActivationInputTag = FGameplayTag::RequestGameplayTag("Input.Organ.2");
    bIsToggleAbility = true;
    
    ResonanceCost = 10.0f; // Per second
    CooldownDuration = 0.0f;
}

void UGA_NeuralLoom_PastVision::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (!Character) 
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    bIsVisionActive = true;
    
    // Apply past vision post-process material
    if (PastVisionMaterial && Character->GetFollowCamera())
    {
        PastVisionMID = UMaterialInstanceDynamic::Create(PastVisionMaterial, Character);
        Character->GetFollowCamera()->AddOrUpdatePostProcessMaterial(FPostProcessMaterial(PassVisionMID), 0);
        
        // Animate transition
        if (PastVisionMID)
        {
            PastVisionMID->SetScalarParameterValue(TEXT("Transition"), 0.0f);
            // Animate to 1.0 over TransitionDuration
        }
    }
    
    // VFX
    if (VisionVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(VisionVFX, Character->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
    }
    
    // Start resonance drain timer
    GetWorld()->GetTimerManager().SetTimer(ResonanceDrainTimer, this, &UGA_NeuralLoom_PastVision::OnResonanceDepleted, 1.0f, true);
    
    Character->OnOrganActivated(OrganType);
    OnOrganActivated(Handle, ActorInfo, ActivationInfo);
}

void UGA_NeuralLoom_PastVision::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    bIsVisionActive = false;
    
    GetWorld()->GetTimerManager().ClearTimer(ResonanceDrainTimer);
    
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Remove post-process material
        if (Character->GetFollowCamera())
        {
            Character->GetFollowCamera()->RemovePostProcessMaterial(FPostProcessMaterial(PastVisionMID));
        }
        
        Character->OnOrganEnded(OrganType);
    }
    
    PastVisionMID = nullptr;
    
    OnOrganEnded(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_NeuralLoom_PastVision::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }
    
    // Check resonance
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character && Character->GetAbilitySystemComponent())
    {
        FGameplayTag ResonanceTag = FGameplayTag::RequestGameplayTag("Attribute.Resonance");
        float CurrentResonance = Character->GetAbilitySystemComponent()->GetNumericAttribute(ResonanceTag);
        return CurrentResonance >= ResonanceCostPerSecond;
    }
    
    return false;
}

void UGA_NeuralLoom_PastVision::OnResonanceDepleted()
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (Character && Character->GetAbilitySystemComponent())
    {
        FGameplayTag ResonanceTag = FGameplayTag::RequestGameplayTag("Attribute.Resonance");
        float CurrentResonance = Character->GetAbilitySystemComponent()->GetNumericAttribute(ResonanceTag);
        
        if (CurrentResonance < ResonanceCostPerSecond)
        {
            // Auto-deactivate
            K2_EndAbility();
        }
    }
}