// MassCalculatorActor.cpp
#include "MassCalculatorActor.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsSettings.h"

AMassCalculatorActor::AMassCalculatorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // ��ʼ����ײ����
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); // ����������ײ
}

float AMassCalculatorActor::CalculateTotalMass() const
{
    float TotalMass = 0.0f;
    UWorld* World = GetWorld();
    if (!World || !BoxComponent) return TotalMass;

    // ��ȡ���ӵļ��β���
    FVector Location = BoxComponent->GetComponentLocation();
    FRotator Rotation = BoxComponent->GetComponentRotation();
    FVector Extent = BoxComponent->GetScaledBoxExtent();
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(Extent);

    // ���ò�ѯ����������������������������
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody); // ������Ŀ���õ���

    // ִ���ص���ѯ
    TArray<FOverlapResult> Overlaps;
    World->OverlapMultiByObjectType(
        Overlaps,
        Location,
        Rotation.Quaternion(),
        ObjectParams,
        CollisionShape,
        QueryParams
    );

    // �������н�����ۼ�����
    for (const FOverlapResult& Overlap : Overlaps)
    {
        UPrimitiveComponent* Component = Overlap.GetComponent();
        CheckComponentAndAddMass(Component, TotalMass);
    }

    return TotalMass;
}

void AMassCalculatorActor::CheckComponentAndAddMass(UPrimitiveComponent* Component, float& TotalMass) const
{
    if (Component && Component->IsSimulatingPhysics())
    {
        TotalMass += Component->GetMass();
    }
}