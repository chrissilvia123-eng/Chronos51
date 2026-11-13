// Astra Core Synthesis Implementation

#include "GA_AstraCore_Synthesis.h"
#include "Chronos51Character.h"
#include "Chronos51GameInstance.h"
#include "TimelineManager.h"
#include "PuzzleManager.h"
#include "SaveSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"

UGA_AstraCore_Synthesis::UGA_AstraCore_Synthesis()
{
    OrganType = EOrganType::AstraCore;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ.AstraCore"));
    
    ActivationInputTag = FGameplayTag::RequestGameplayTag("Input.Organ.5");
    bIsToggleAbility = false;
    
    ResonanceCost = 0.0f;
    CooldownDuration = 0.0f;
}

bool UGA_AstraCore_Synthesis::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }
    
    return CheckRequirements();
}

void UGA_AstraCore_Synthesis::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    if (!CheckRequirements())
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
    
    CurrentSynthesisStage = 0;
    
    // Start synthesis sequence
    if (SynthesisSequence)
    {
        ALevelSequenceActor* SequenceActor = nullptr;
        ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SynthesisSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
        
        if (Player && SequenceActor)
        {
            this->SequenceActor = SequenceActor;
            Player->Play();
            
            // Bind to sequence events
            // In practice, use sequence event tracks to trigger stage completion
        }
    }
    
    // VFX
    if (SynthesisVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SynthesisVFX, Character->GetActorLocation());
    }
    
    Character->OnOrganActivated(OrganType);
    OnOrganActivated(Handle, ActorInfo, ActivationInfo);
}

void UGA_AstraCore_Synthesis::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    AChronos51Character* Character = Cast<AChronos51Character>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        Character->OnOrganEnded(OrganType);
    }
    
    if (SequenceActor)
    {
        SequenceActor->Destroy();
        SequenceActor = nullptr;
    }
    
    OnOrganEnded(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_AstraCore_Synthesis::OnSynthesisStageComplete(int32 Stage)
{
    CurrentSynthesisStage = Stage;
    
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return;
    
    // Play organ merge VFX
    if (Stage > 0 && Stage <= 4 && OrganMergeVFX[Stage - 1])
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrganMergeVFX[Stage - 1], Character->GetActorLocation());
    }
    
    // Check if final stage
    if (Stage >= 5)
    {
        DetermineEnding();
    }
}

void UGA_AstraCore_Synthesis::DetermineEnding()
{
    // Get save system to check ending flags
    UChronos51GameInstance* GameInstance = Cast<UChronos51GameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance) return;
    
    USaveSystem* SaveSystem = GameInstance->GetSaveSystem();
    if (!SaveSystem) return;
    
    // Determine ending based on Trust x Memory Fragments matrix
    // Ending 1: Low Trust, Low Fragments -> Astra Lost, Ship Destroyed
    // Ending 2: High Trust, Low Fragments -> Astra Freed, Ship Lost
    // Ending 3: Low Trust, High Fragments -> Corwin Wins, Cycle Continues
    // Ending 4: High Trust, High Fragments -> True Ending: Unity
    
    int32 TrustLevel = 0; // Get from AstraCompanion
    int32 MemoryFragments = 0; // Get from SaveSystem
    
    int32 EndingIndex = 0;
    
    if (TrustLevel >= 8 && MemoryFragments >= 12)
    {
        EndingIndex = 3; // True Ending
    }
    else if (TrustLevel >= 8 && MemoryFragments < 12)
    {
        EndingIndex = 1; // Astra Freed
    }
    else if (TrustLevel < 8 && MemoryFragments >= 12)
    {
        EndingIndex = 2; // Corwin Wins
    }
    else
    {
        EndingIndex = 0; // Tragedy
    }
    
    PlayEndingSequence(EndingIndex);
}

void UGA_AstraCore_Synthesis::PlayEndingSequence(int32 EndingIndex)
{
    if (EndingIndex >= 0 && EndingIndex < EndingSequences.Num() && EndingSequences[EndingIndex])
    {
        ALevelSequenceActor* SequenceActor = nullptr;
        ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), EndingSequences[EndingIndex], FMovieSceneSequencePlaybackSettings(), SequenceActor);
        
        if (Player)
        {
            Player->Play();
        }
    }
    
    // Set ending flag in save system
    UChronos51GameInstance* GameInstance = Cast<UChronos51GameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        USaveSystem* SaveSystem = GameInstance->GetSaveSystem();
        if (SaveSystem)
        {
            SaveSystem->SetEndingFlag(FString::Printf(TEXT("Ending_%d"), EndingIndex), true);
        }
    }
}

bool UGA_AstraCore_Synthesis::CheckRequirements() const
{
    AChronos51Character* Character = Cast<AChronos51Character>(GetAvatarActorFromActorInfo());
    if (!Character) return false;
    
    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC) return false;
    
    // Check all four organs
    for (EOrganType Organ : RequiredOrgans)
    {
        FGameplayTag OrganTag = UOrganAbilityBase::GetOrganTag(Organ);
        if (!ASC->HasMatchingGameplayTag(OrganTag))
        {
            return false;
        }
    }
    
    // Check trust level (via AstraCompanion)
    // Check memory fragments (via SaveSystem)
    // These would be checked via the GameInstance subsystems
    
    return true;
}