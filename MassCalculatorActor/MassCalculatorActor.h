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

    // �����������ĺ���
    UFUNCTION(BlueprintCallable, Category = "Physics")
    float CalculateTotalMass() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;

private:
    // �����������������Ƿ���Ч���ۼ�����
    void CheckComponentAndAddMass(UPrimitiveComponent* Component, float& TotalMass) const;
};