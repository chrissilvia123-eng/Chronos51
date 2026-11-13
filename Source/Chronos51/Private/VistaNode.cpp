// Copyright Chronos51 Studios 2026

#include "VistaNode.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Camera/CineCameraComponent.h"
#include "Chronos51Character.h"
#include "TimelineManager.h"
#include "Kismet/GameplayStatics.h"

AVistaNode::AVistaNode()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root component
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Cinematic camera
    CinematicCamera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CinematicCamera"));
    CinematicCamera->SetupAttachment(RootComponent);
    CinematicCamera->SetActive(false);
    CinematicCamera->FieldOfView = 90.0f;
    CinematicCamera->CurrentFocalLength = 35.0f;
    CinematicCamera->FocusSettings.ManualFocusDistance = 500.0f;

    // Trigger volume
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    TriggerVolume->SetupAttachment(RootComponent);
    TriggerVolume->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    TriggerVolume->SetGenerateOverlapEvents(true);

    // Camera path (optional)
    CameraPath = CreateDefaultSubobject<USplineComponent>(TEXT("CameraPath"));
    CameraPath->SetupAttachment(RootComponent);
    CameraPath->SetHiddenInGame(true);
}

void AVistaNode::BeginPlay()
{
    Super::BeginPlay();

    // Bind overlap events
    if (TriggerVolume)
    {
        TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AVistaNode::OnPlayerEnterTrigger);
        TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AVistaNode::OnPlayerExitTrigger);
    }

    // Register with TimelineManager
    if (UGameInstance* GI = GetGameInstance())
    {
        UTimelineManager* TimelineManager = GI->GetSubsystem<UTimelineManager>();
        if (TimelineManager)
        {
            // Registration would happen here
        }
    }
}

void AVistaNode::OnPlayerEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& SweepResult)
{
    AChronos51Character* Player = Cast<AChronos51Character>(OtherActor);
    if (Player)
    {
        bPlayerInRange = true;
        Player->SetCurrentVistaNode(this); // Would need this method on character
    }
}

void AVistaNode::OnPlayerExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AChronos51Character* Player = Cast<AChronos51Character>(OtherActor);
    if (Player)
    {
        bPlayerInRange = false;
        Player->SetCurrentVistaNode(nullptr);
    }
}

void AVistaNode::ActivateNode()
{
    if (bIsActivated) return;

    bIsActivated = true;
    OnNodeActivated.Broadcast(this);

    // Unlock fast travel for this zone
    if (UGameInstance* GI = GetGameInstance())
    {
        UTimelineManager* TimelineManager = GI->GetSubsystem<UTimelineManager>();
        if (TimelineManager)
        {
            TimelineManager->UnlockFastTravel(ZoneID);
            TimelineManager->ActivatedVistaNodes.AddUnique(NodeID);
        }
    }
}

void AVistaNode::ActivateCinematicCamera()
{
    if (CinematicCamera)
    {
        CinematicCamera->SetActive(true, true);
    }
}

void AVistaNode::DeactivateCinematicCamera()
{
    if (CinematicCamera)
    {
        CinematicCamera->SetActive(false);
    }
}

bool AVistaNode::CanPhaseShift() const
{
    if (!bIsActivated) return false; // Must activate first (or allow from range)
    
    if (RequiredOrgan != NAME_None)
    {
        // Check if player has required organ
        // Would need access to player's OwnedOrgans
    }

    return true;
}

FTransform AVistaNode::GetPhaseAnchorTransform() const
{
    // Return transform for Phase Shift spawn point
    return GetActorTransform();
}

void AVistaNode::SetFastTravelUnlocked(bool bUnlocked)
{
    bIsFastTravelUnlocked = bUnlocked;
}