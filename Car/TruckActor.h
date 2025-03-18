// TruckActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "TruckActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTruckMovementStateChanged);

UCLASS()
class  ATruckActor : public AActor
{
    GENERATED_BODY()

public:
    ATruckActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // �ƶ����ƺ���
    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void StopMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void StartMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void ToggleMovement();

    UFUNCTION(BlueprintPure, Category = "Truck|Control")
    bool IsMoving() const { return bIsMoving; }

    // �¼�ί��
    UPROPERTY(BlueprintAssignable, Category = "Truck|Events")
    FOnTruckMovementStateChanged OnMovementStarted;

    UPROPERTY(BlueprintAssignable, Category = "Truck|Events")
    FOnTruckMovementStateChanged OnMovementStopped;

    // �ɱ༭����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Truck|Settings")
    float MoveSpeed = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Truck|Settings")
    bool bLoopPath = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Truck|Settings", meta = (ClampMin = "0.0"))
    float Acceleration = 500.0f;

private:
    void MoveAlongSpline(float DeltaTime);

    UPROPERTY(VisibleAnywhere, Category = "Truck|Components")
    USplineComponent* SplineComponent;

    UPROPERTY(VisibleAnywhere, Category = "Truck|Components")
    UStaticMeshComponent* TruckMesh;

    // ����ʱ״̬
    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    float CurrentSplineDistance;

    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    float CurrentEffectiveSpeed;

    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    bool bIsMoving;
};