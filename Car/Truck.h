#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Truck.generated.h"

UCLASS()
class  ATruck : public AActor
{
    GENERATED_BODY()

public:
    ATruck();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Truck")
    void StartMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck")
    void PauseMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck")
    void SetShouldLoop(bool bLoop);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementSpeed = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float StopTolerance = 1.0f; // µ¥Î»£ºÀåÃ×

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

    TArray<TWeakObjectPtr<APawn>> Passengers;
    FVector PreviousLocation;
    float CurrentDistance = 0.0f;
    bool bIsMoving = false;
    bool bShouldLoop = true;

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};