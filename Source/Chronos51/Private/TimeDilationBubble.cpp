// Time Dilation Bubble Implementation

#include "TimeDilationBubble.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATimeDilationBubble::ATimeDilationBubble()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(Radius);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    SphereComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
    RootComponent = SphereComponent;

    VFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComponent"));
    VFXComponent->SetupAttachment(RootComponent);
    VFXComponent->bAutoActivate = false;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetIsUISound(false);
    AudioComponent->SetVolumeMultiplier(1.0f);

    SetReplicates(true);
    NetUpdateFrequency = 10.0f;
}

void ATimeDilationBubble::Initialize(float InDilationFactor, float InRadius, float InDuration)
{
    TimeDilationFactor = InDilationFactor;
    Radius = InRadius;
    Duration = InDuration;
    TimeRemaining = Duration;

    SphereComponent->SetSphereRadius(Radius);
    
    if (VFXComponent && VFXComponent->GetAsset())
    {
        VFXComponent->Activate(true);
        VFXComponent->SetVariableFloat(TEXT("Radius"), Radius);
        VFXComponent->SetVariableFloat(TEXT("DilationFactor"), TimeDilationFactor);
    }
    
    if (AudioComponent && AudioComponent->Sound)
    {
        AudioComponent->Play();
    }
}

void ATimeDilationBubble::SetInstigator(AActor* InInstigator)
{
    InstigatorActor = InInstigator;
    if (InstigatorActor)
    {
        IgnoredActors.AddUnique(InstigatorActor);
    }
}

void ATimeDilationBubble::BeginPlay()
{
    Super::BeginPlay();
    
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimeDilationBubble::OnOverlapBegin);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATimeDilationBubble::OnOverlapEnd);
    
    // Initial overlap check
    TArray<AActor*> OverlappingActors;
    SphereComponent->GetOverlappingActors(OverlappingActors);
    for (AActor* Actor : OverlappingActors)
    {
        OnOverlapBegin(SphereComponent, Actor, nullptr, 0, false, FHitResult());
    }
}

void ATimeDilationBubble::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    TimeRemaining -= DeltaTime;
    float Progress = 1.0f - (TimeRemaining / Duration);
    UpdateVFX(Progress);
    
    if (TimeRemaining <= 0.0f)
    {
        ExpireBubble();
    }
}

void ATimeDilationBubble::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Restore time dilation for all affected actors
    for (auto& Pair : OriginalTimeDilation)
    {
        if (Pair.Key && Pair.Key->IsValidLowLevel())
        {
            RestoreTimeDilation(Pair.Key);
        }
    }
    OriginalTimeDilation.Empty();
    
    Super::EndPlay(EndPlayReason);
}

void ATimeDilationBubble::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || OtherActor == InstigatorActor) return;
    if (IgnoredActors.Contains(OtherActor)) return;
    
    // Check object type
    bool bValidType = false;
    for (auto ObjectType : AffectedObjectTypes)
    {
        if (OtherActor->GetObjectType() == ObjectType)
        {
            bValidType = true;
            break;
        }
    }
    if (!bValidType && AffectedObjectTypes.Num() > 0) return;
    
    ApplyTimeDilation(OtherActor, TimeDilationFactor);
}

void ATimeDilationBubble::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || OtherActor == this) return;
    RestoreTimeDilation(OtherActor);
}

void ATimeDilationBubble::ApplyTimeDilation(AActor* TargetActor, float Factor)
{
    if (!TargetActor) return;
    
    // Store original
    UWorld* World = TargetActor->GetWorld();
    if (World)
    {
        float OriginalDilation = World->GetWorldSettings()->GetEffectiveTimeDilation();
        OriginalTimeDilation.Add(TargetActor, OriginalDilation);
    }
    
    // Apply to CharacterMovementComponent
    if (ACharacter* Character = Cast<ACharacter>(TargetActor))
    {
        if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
        {
            Movement->SetCustomTimeDilation(Factor);
        }
    }
    
    // Apply to any component with CustomTimeDilation
    TArray<UActorComponent*> Components;
    TargetActor->GetComponents(Components);
    for (UActorComponent* Comp : Components)
    {
        if (Comp->GetClass()->ImplementsInterface(UCustomTimeDilationInterface::StaticClass()))
        {
            ICustomTimeDilationInterface::Execute_SetCustomTimeDilation(Comp, Factor);
        }
    }
}

void ATimeDilationBubble::RestoreTimeDilation(AActor* TargetActor)
{
    if (!TargetActor) return;
    
    float* OriginalDilation = OriginalTimeDilation.Find(TargetActor);
    if (OriginalDilation)
    {
        // Restore CharacterMovementComponent
        if (ACharacter* Character = Cast<ACharacter>(TargetActor))
        {
            if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
            {
                Movement->SetCustomTimeDilation(*OriginalDilation);
            }
        }
        
        // Restore components
        TArray<UActorComponent*> Components;
        TargetActor->GetComponents(Components);
        for (UActorComponent* Comp : Components)
        {
            if (Comp->GetClass()->ImplementsInterface(UCustomTimeDilationInterface::StaticClass()))
            {
                ICustomTimeDilationInterface::Execute_SetCustomTimeDilation(Comp, *OriginalDilation);
            }
        }
        
        OriginalTimeDilation.Remove(TargetActor);
    }
}

void ATimeDilationBubble::ExpireBubble()
{
    OnExpired.Broadcast();
    Destroy();
}

void ATimeDilationBubble::UpdateVFX(float Progress)
{
    if (VFXComponent && VFXComponent->GetAsset())
    {
        VFXComponent->SetVariableFloat(TEXT("Progress"), Progress);
        VFXComponent->SetVariableFloat(TEXT("Radius"), Radius * (1.0f + Progress * 0.2f)); // Slight expansion
    }
}