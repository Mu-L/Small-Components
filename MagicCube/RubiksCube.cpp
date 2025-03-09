#include "RubiksCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARubiksCube::ARubiksCube()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��ʼ�������
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // ����������������Դ
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cube"));
    if (MeshFinder.Succeeded())
    {
        CubeMesh = MeshFinder.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load cube mesh!"));
    }
}

void ARubiksCube::BeginPlay()
{
    Super::BeginPlay();
    InitializeCubes();
}

void ARubiksCube::InitializeCubes()
{
    if (!CubeMesh) return;

    // ����3x3x3���������
    for (int32 x = 0; x < 3; x++) {
        for (int32 y = 0; y < 3; y++) {
            for (int32 z = 0; z < 3; z++) {
                FCubeData NewCube;

                // ������̬�������
                NewCube.Mesh = NewObject<UStaticMeshComponent>(this);
                NewCube.Mesh->SetStaticMesh(CubeMesh);
                NewCube.Mesh->SetupAttachment(RootComponent);
                NewCube.Mesh->RegisterComponent();

                // ���ó�ʼλ�ú�����
                NewCube.LogicalPos = FIntVector(x, y, z);
                NewCube.Mesh->SetRelativeLocation(LogicalToWorld(NewCube.LogicalPos));
                NewCube.Mesh->SetRelativeScale3D(FVector(0.95f));

                Cubes.Add(NewCube);
            }
        }
    }
}

FVector ARubiksCube::LogicalToWorld(FIntVector LogicalPos) const
{
    // �߼�����ת�������꣨���Ķ��룩
    return FVector(
        (LogicalPos.X - 1) * CubeSize,
        (LogicalPos.Y - 1) * CubeSize,
        (LogicalPos.Z - 1) * CubeSize
    );
}

FIntVector ARubiksCube::WorldToLogical(FVector WorldPos) const
{
    // ��������ת�߼����꣨0-2��Χ��
    return FIntVector(
        FMath::RoundToInt((WorldPos.X / CubeSize) + 1),
        FMath::RoundToInt((WorldPos.Y / CubeSize) + 1),
        FMath::RoundToInt((WorldPos.Z / CubeSize) + 1)
    );
}

void ARubiksCube::StartRotation(FIntVector LayerCenter, bool bClockwise)
{
    if (bIsRotating) return;
    CalculateRotationParams(LayerCenter, bClockwise);
    bIsRotating = true;
}

void ARubiksCube::CalculateRotationParams(FIntVector LayerCenter, bool bClockwise)
{
    CurrentRotation = FRotationData();

    // ����������������
    CurrentRotation.PivotWorld = LogicalToWorld(LayerCenter);

    // ȷ����ת��
    if (LayerCenter.X == 0 || LayerCenter.X == 2) {
        CurrentRotation.RotationAxis = FVector::XAxisVector;
    }
    else if (LayerCenter.Y == 0 || LayerCenter.Y == 2) {
        CurrentRotation.RotationAxis = FVector::YAxisVector;
    }
    else if (LayerCenter.Z == 0 || LayerCenter.Z == 2) {
        CurrentRotation.RotationAxis = FVector::ZAxisVector;
    }

    // ������ת�Ƕȣ����ַ���
    float Angle = bClockwise ? -90.0f : 90.0f;
    CurrentRotation.TargetAngle = Angle;

    // ɸѡ��Ӱ��������
    for (int32 i = 0; i < Cubes.Num(); i++) {
        FVector CubeWorldPos = Cubes[i].Mesh->GetComponentLocation();
        if (IsInRotationLayer(CubeWorldPos, LayerCenter)) {
            CurrentRotation.AffectedIndices.Add(i);
        }
    }
}

bool ARubiksCube::IsInRotationLayer(const FVector& CubeWorldPos, FIntVector LayerCenter) const
{
    FIntVector CubeLogical = WorldToLogical(CubeWorldPos);

    // �ж��Ƿ���Ŀ���
    if (LayerCenter.X == 0 || LayerCenter.X == 2)
        return CubeLogical.X == LayerCenter.X;
    if (LayerCenter.Y == 0 || LayerCenter.Y == 2)
        return CubeLogical.Y == LayerCenter.Y;
    if (LayerCenter.Z == 0 || LayerCenter.Z == 2)
        return CubeLogical.Z == LayerCenter.Z;

    return false;
}

void ARubiksCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsRotating) return;

    // ������ת������������������ϵ��ת��
    const float RotationSpeed = 180.0f;
    float DeltaAngle = FMath::Clamp(
        RotationSpeed * DeltaTime,
        0.0f,
        FMath::Abs(CurrentRotation.TargetAngle - CurrentRotation.CurrentAngle)
    ) * FMath::Sign(CurrentRotation.TargetAngle);

    // ��������ռ���ת��Ԫ��
    FQuat WorldRotationDelta = FQuat(CurrentRotation.RotationAxis,
        FMath::DegreesToRadians(DeltaAngle));

    // Ӧ����ת��ÿ����Ӱ��������
    for (int32 Index : CurrentRotation.AffectedIndices) {
        FCubeData& Cube = Cubes[Index];

        // ����������ĵ�λ��
        FVector RelativePos = Cube.Mesh->GetComponentLocation() - CurrentRotation.PivotWorld;

        // Ӧ������ռ���ת��������ת��ʼ��Ϊ��������ϵ����
        FVector NewRelativePos = WorldRotationDelta.RotateVector(RelativePos);
        Cube.Mesh->SetWorldLocation(CurrentRotation.PivotWorld + NewRelativePos);

        // ������ת����������ռ���ת�ۻ���
        FQuat NewWorldRotation = WorldRotationDelta * Cube.Mesh->GetComponentQuat();
        Cube.Mesh->SetWorldRotation(NewWorldRotation);
    }

    CurrentRotation.CurrentAngle += DeltaAngle;

    // �����ת����
    if (FMath::Abs(CurrentRotation.CurrentAngle) >= FMath::Abs(CurrentRotation.TargetAngle)) {
        FinalizeRotation();
        bIsRotating = false;
    }

    // ���Ƶ�ǰ��ת��
    DrawDebugLine(GetWorld(),
        CurrentRotation.PivotWorld,
        CurrentRotation.PivotWorld + CurrentRotation.RotationAxis * 200,
        FColor::Green,
        false, -1, 0, 2.0f);

    // ��ʾ��������ת�Ƕ�
    for (const FCubeData& Cube : Cubes) {
        FRotator Rot = Cube.Mesh->GetComponentRotation();
        FString Text = FString::Printf(TEXT("P=%.0f,Y=%.0f,R=%.0f"),
            Rot.Pitch, Rot.Yaw, Rot.Roll);
        DrawDebugString(GetWorld(), Cube.Mesh->GetComponentLocation(),
            Text, nullptr, FColor::White, 0.0f);
    }
}

void ARubiksCube::FinalizeRotation()
{  
    // �����߼�����
    for (int32 Index : CurrentRotation.AffectedIndices) {
        FCubeData& Cube = Cubes[Index];
        Cube.LogicalPos = WorldToLogical(Cube.Mesh->GetComponentLocation());
    }

    // ǿ�ƶ��뵽���񲢱�׼����ת
    for (FCubeData& Cube : Cubes) {
        // ����λ��
        FVector AlignedPos = LogicalToWorld(Cube.LogicalPos);
        Cube.Mesh->SetWorldLocation(AlignedPos);

        // ��׼����ת���ֽ�Ϊ90����������
        FRotator NormalizedRot = Cube.Mesh->GetComponentRotation().GetNormalized();
        NormalizedRot.Pitch = FMath::GridSnap(NormalizedRot.Pitch, 90.0f);
        NormalizedRot.Yaw = FMath::GridSnap(NormalizedRot.Yaw, 90.0f);
        NormalizedRot.Roll = FMath::GridSnap(NormalizedRot.Roll, 90.0f);
        Cube.Mesh->SetWorldRotation(NormalizedRot);
        Cube.CurrentRotation = NormalizedRot;
    }
}