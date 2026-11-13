// Organ Ability Base Implementation

#include "OrganAbilityBase.h"
#include "AbilitySystemComponent.h"
#include "Chronos51Character.h"

UOrganAbilityBase::UOrganAbilityBase()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    
    // Default tags
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Organ"));
}

void UOrganAbilityBase::OnOrganActivated_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    // Override in derived classes
}

void UOrganAbilityBase::OnOrganEnded_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicated, bool bWasCancelled)
{
    // Override in derived classes
}

bool UOrganAbilityBase::HasOrgan(const UAbilitySystemComponent* ASC, EOrganType RequiredOrgan)
{
    if (!ASC) return false;
    
    FGameplayTag OrganTag = GetOrganTag(RequiredOrgan);
    return ASC->HasMatchingGameplayTag(OrganTag);
}

FGameplayTag UOrganAbilityBase::GetOrganTag(EOrganType OrganType)
{
    switch (OrganType)
    {
        case EOrganType::ChronosCore:  return FGameplayTag::RequestGameplayTag("Organ.ChronosCore");
        case EOrganType::NeuralLoom:   return FGameplayTag::RequestGameplayTag("Organ.NeuralLoom");
        case EOrganType::VoidSail:     return FGameplayTag::RequestGameplayTag("Organ.VoidSail");
        case EOrganType::GravityAnchor:return FGameplayTag::RequestGameplayTag("Organ.GravityAnchor");
        case EOrganType::AstraCore:    return FGameplayTag::RequestGameplayTag("Organ.AstraCore");
        default:                       return FGameplayTag();
    }
}