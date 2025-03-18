// TruckActor.cpp
#include "TruckActor.h"
#include "Components/SplineComponent.h"

ATruckActor::ATruckActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��ʼ�����
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;

    TruckMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TruckMesh"));
    TruckMesh->SetupAttachment(RootComponent);

    // ��ʼ������ʱ����
    CurrentSplineDistance = 0.0f;
    CurrentEffectiveSpeed = MoveSpeed;
    bIsMoving = true;
}

void ATruckActor::BeginPlay()
{
    Super::BeginPlay();
    // ȷ����ʼλ����ȷ
    if (SplineComponent->GetNumberOfSplinePoints() > 1)
    {
        const FVector InitialLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
        const FRotator InitialRotation = SplineComponent->GetRotationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
        TruckMesh->SetWorldLocationAndRotation(InitialLocation, InitialRotation);
    }
}

void ATruckActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // �ٶȲ�ֵ����
    const float TargetSpeed = bIsMoving ? MoveSpeed : 0.0f;
    const float InterpSpeed = (Acceleration > 0.0f) ? (MoveSpeed / Acceleration) : 10.0f;

    CurrentEffectiveSpeed = FMath::FInterpTo(
        CurrentEffectiveSpeed,
        TargetSpeed,
        DeltaTime,
        InterpSpeed
    );

    // ������Ч�ƶ�ʱ����λ��
    if (!FMath::IsNearlyZero(CurrentEffectiveSpeed, 0.1f))
    {
        MoveAlongSpline(DeltaTime);
    }
}

void ATruckActor::StopMoving()
{
    if (bIsMoving)
    {
        bIsMoving = false;
        OnMovementStopped.Broadcast();
    }
}

void ATruckActor::StartMoving()
{
    if (!bIsMoving)
    {
        bIsMoving = true;
        OnMovementStarted.Broadcast();
    }
}

void ATruckActor::ToggleMovement()
{
    bIsMoving ? StopMoving() : StartMoving();
}

void ATruckActor::MoveAlongSpline(float DeltaTime)
{
    if (SplineComponent->GetNumberOfSplinePoints() < 2) return;

    // �����ƶ�����
    const float SplineLength = SplineComponent->GetSplineLength();
    CurrentSplineDistance += CurrentEffectiveSpeed * DeltaTime;

    // ����ѭ���߼�
    if (bLoopPath)
    {
        CurrentSplineDistance = FMath::Fmod(CurrentSplineDistance, SplineLength);
        if (CurrentSplineDistance < 0.0f)
        {
            CurrentSplineDistance += SplineLength;
        }
    }
    else
    {
        CurrentSplineDistance = FMath::Clamp(CurrentSplineDistance, 0.0f, SplineLength);
    }

    // ��ȡ�任����
    const FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
        CurrentSplineDistance,
        ESplineCoordinateSpace::World
    );

    const FRotator NewRotation = SplineComponent->GetRotationAtDistanceAlongSpline(
        CurrentSplineDistance,
        ESplineCoordinateSpace::World
    );

    // Ӧ�ñ任
    TruckMesh->SetWorldLocationAndRotation(NewLocation, NewRotation);
}