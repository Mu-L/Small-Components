// MassCalculatorActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MassCalculatorActor.generated.h"

class UBoxComponent;

UCLASS()
class DEMO_API AMassCalculatorActor : public AActor
{
    GENERATED_BODY()

public:
    AMassCalculatorActor();

    // 计算总质量的函数
    UFUNCTION(BlueprintCallable, Category = "Physics")
    float CalculateTotalMass() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;

private:
    // 辅助函数：检查组件是否有效并累加质量
    void CheckComponentAndAddMass(UPrimitiveComponent* Component, float& TotalMass) const;
};