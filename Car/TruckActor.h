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

    // 移动控制函数
    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void StopMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void StartMoving();

    UFUNCTION(BlueprintCallable, Category = "Truck|Control")
    void ToggleMovement();

    UFUNCTION(BlueprintPure, Category = "Truck|Control")
    bool IsMoving() const { return bIsMoving; }

    // 事件委托
    UPROPERTY(BlueprintAssignable, Category = "Truck|Events")
    FOnTruckMovementStateChanged OnMovementStarted;

    UPROPERTY(BlueprintAssignable, Category = "Truck|Events")
    FOnTruckMovementStateChanged OnMovementStopped;

    // 可编辑参数
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

    // 运行时状态
    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    float CurrentSplineDistance;

    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    float CurrentEffectiveSpeed;

    UPROPERTY(VisibleInstanceOnly, Category = "Truck|Runtime")
    bool bIsMoving;
};