// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VistaNode.generated.h"

class UCineCameraComponent;
class UBoxComponent;
class USplineComponent;
class ULevelSequence;

UCLASS()
class CHRONOS51_API AVistaNode : public AActor
{
    GENERATED_BODY()

public:
    AVistaNode();

    virtual void BeginPlay() override;

    // Node identity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node")
    FName NodeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node")
    FName ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node")
    FText DisplayName;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vista Node|Camera")
    TObjectPtr<UCineCameraComponent> CinematicCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vista Node|Camera")
    TObjectPtr<USplineComponent> CameraPath; // For multi-angle nodes

    // Trigger
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vista Node|Trigger")
    TObjectPtr<UBoxComponent> TriggerVolume;

    // Phase Shift
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Phase Shift")
    TObjectPtr<ULevelSequence> PhaseShiftSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Phase Shift")
    float PhaseShiftDuration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Phase Shift")
    bool bInstantCutAccessible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Phase Shift")
    bool bReducedMotionAccessible = true;

    // Connections
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Network")
    TArray<FName> ConnectedNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Network")
    TArray<FName> UnlocksPuzzles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vista Node|Network")
    FName RequiredOrgan = NAME_None;

    // State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vista Node|State")
    bool bIsActivated = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vista Node|State")
    bool bIsFastTravelUnlocked = false;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    void ActivateNode();

    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    void ActivateCinematicCamera();

    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    void DeactivateCinematicCamera();

    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    bool CanPhaseShift() const;

    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    FTransform GetPhaseAnchorTransform() const;

    UFUNCTION(BlueprintCallable, Category = "Vista Node")
    void SetFastTravelUnlocked(bool bUnlocked);

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeActivated, AVistaNode*, Node);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseShiftInitiated, AVistaNode*, Node);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseShiftCompleted, AVistaNode*, Node);

    UPROPERTY(BlueprintAssignable, Category = "Vista Node|Events")
    FOnNodeActivated OnNodeActivated;

    UPROPERTY(BlueprintAssignable, Category = "Vista Node|Events")
    FOnPhaseShiftInitiated OnPhaseShiftInitiated;

    UPROPERTY(BlueprintAssignable, Category = "Vista Node|Events")
    FOnPhaseShiftCompleted OnPhaseShiftCompleted;

protected:
    UFUNCTION()
    void OnPlayerEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                              bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlayerExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    bool bPlayerInRange = false;
};