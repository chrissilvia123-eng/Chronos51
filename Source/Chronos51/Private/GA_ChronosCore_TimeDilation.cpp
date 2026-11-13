// Chronos Core Time Dilation Implementation

#include "GA_ChronosCore_TimeDilation.h"
#include "TimeDilationBubble.h"
#include "Chronos51Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UGA_ChronosCore_TimeDilation::UGA_ChronosCore_TimeDilation()
{
    OrganType = EOrganType::ChronosCore;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ.ChronosCore"));
    
    // Input binding
    ActivationInputTag = FGameplayTag::RequestGameplayTag("Input.Organ.1");
    
    // Cost
    ResonanceCost = 50.0f;
    CooldownDuration = 30.0f;
}

void UGA_ChronosCore_TimeDilation::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
    
    // Spawn time dilation bubble at character location
    FVector SpawnLocation = Character->GetActorLocation();
    FRotator SpawnRotation = FRotator::ZeroRotator;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Character;
    SpawnParams.Instigator = Character;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    ActiveBubble = GetWorld()->SpawnActor<ATimeDilationBubble>(BubbleClass, SpawnLocation, SpawnRotation, SpawnParams);
    
    if (ActiveBubble)
    {
        ActiveBubble->Initialize(TimeDilationFactor, BubbleRadius, Duration);
        ActiveBubble->SetInstigator(Character);
        ActiveBubble->OnExpired.AddDynamic(this, &UGA_ChronosCore_TimeDilation::OnBubbleExpired);
    }
    
    // VFX
    if (ActivationVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActivationVFX, SpawnLocation, SpawnRotation);
    }
    
    // SFX
    if (ActivationSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActivationSound, SpawnLocation);
    }
    
    // Notify character
    Character->OnOrganActivated(OrganType);
    
    OnOrganActivated(Handle, ActorInfo, ActivationInfo);
}

void UGA_ChronosCore_TimeDilation::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (ActiveBubble && ActiveBubble->IsValidLowLevel())
    {
        ActiveBubble->OnExpired.RemoveDynamic(this, &UGA_ChronosCore_TimeDilation::OnBubbleExpired);
        ActiveBubble->Destroy();
        ActiveBubble = nullptr;
    }
    
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        Character->OnOrganEnded(OrganType);
    }
    
    OnOrganEnded(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ChronosCore_TimeDilation::OnBubbleExpired()
{
    ActiveBubble = nullptr;
}