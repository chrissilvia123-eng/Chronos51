// Time Dilation Bubble Actor - Spawned by Chronos Core ability

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeDilationBubble.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class CHRONOS51_API ATimeDilationBubble : public AActor
{
    GENERATED_BODY()

public:
    ATimeDilationBubble();

    // Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
    float TimeDilationFactor = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
    float Radius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
    float Duration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
    TArray<TEnumAsByte<EObjectTypeQuery>> AffectedObjectTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
    TArray<AActor*> IgnoredActors;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* VFXComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* AudioComponent;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Time Dilation")
    FOnTimeDilationBubbleExpired OnExpired;

    // Public interface
    UFUNCTION(BlueprintCallable, Category = "Time Dilation")
    void Initialize(float InDilationFactor, float InRadius, float InDuration);

    UFUNCTION(BlueprintCallable, Category = "Time Dilation")
    void SetInstigator(AActor* InInstigator);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY()
    AActor* InstigatorActor;

    UPROPERTY()
    float TimeRemaining = 0.0f;

    UPROPERTY()
    TMap<AActor*, float> OriginalTimeDilation;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void ApplyTimeDilation(AActor* TargetActor, float Factor);

    UFUNCTION()
    void RestoreTimeDilation(AActor* TargetActor);

    UFUNCTION()
    void ExpireBubble();

    UFUNCTION()
    void UpdateVFX(float Progress);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeDilationBubbleExpired);