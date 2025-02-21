// MassCalculatorActor.cpp
#include "MassCalculatorActor.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsSettings.h"

AMassCalculatorActor::AMassCalculatorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 初始化碰撞盒子
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); // 禁用自身碰撞
}

float AMassCalculatorActor::CalculateTotalMass() const
{
    float TotalMass = 0.0f;
    UWorld* World = GetWorld();
    if (!World || !BoxComponent) return TotalMass;

    // 获取盒子的几何参数
    FVector Location = BoxComponent->GetComponentLocation();
    FRotator Rotation = BoxComponent->GetComponentRotation();
    FVector Extent = BoxComponent->GetScaledBoxExtent();
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(Extent);

    // 配置查询参数：忽略自身，检测所有物理对象
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody); // 根据项目设置调整

    // 执行重叠查询
    TArray<FOverlapResult> Overlaps;
    World->OverlapMultiByObjectType(
        Overlaps,
        Location,
        Rotation.Quaternion(),
        ObjectParams,
        CollisionShape,
        QueryParams
    );

    // 遍历所有结果并累加质量
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