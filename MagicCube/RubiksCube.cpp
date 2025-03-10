#include "RubiksCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARubiksCube::ARubiksCube()
{
    PrimaryActorTick.bCanEverTick = true;

    // 初始化根组件
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // 加载立方体网格资源
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Script/Engine.StaticMesh'/Game/Test/MagicCube.MagicCube'"));
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

    // 游戏开始时自动打乱（开发者可注释此行）
    ScrambleCube(25,true);
}

void ARubiksCube::InitializeCubes()
{
    if (!CubeMesh) return;

    // 生成3x3x3立方体矩阵
    for (int32 x = 0; x < 3; x++) {
        for (int32 y = 0; y < 3; y++) {
            for (int32 z = 0; z < 3; z++) {
                FCubeData NewCube;

                // 创建静态网格组件
                NewCube.Mesh = NewObject<UStaticMeshComponent>(this);
                NewCube.Mesh->SetStaticMesh(CubeMesh);
                NewCube.Mesh->SetupAttachment(RootComponent);
                NewCube.Mesh->RegisterComponent();

                // 设置初始位置和缩放
                NewCube.LogicalPos = FIntVector(x, y, z);
                NewCube.Mesh->SetRelativeLocation(LogicalToWorld(NewCube.LogicalPos));
                NewCube.Mesh->SetRelativeScale3D(FVector(0.5f));

                Cubes.Add(NewCube);
            }
        }
    }
}

FVector ARubiksCube::LogicalToWorld(FIntVector LogicalPos) const
{
    // 逻辑坐标转世界坐标（中心对齐）
    return FVector(
        (LogicalPos.X - 1) * CubeSize,
        (LogicalPos.Y - 1) * CubeSize,
        (LogicalPos.Z - 1) * CubeSize
    );
}

FIntVector ARubiksCube::WorldToLogical(FVector WorldPos) const
{
    // 世界坐标转逻辑坐标（0-2范围）
    return FIntVector(
        FMath::RoundToInt((WorldPos.X / CubeSize) + 1),
        FMath::RoundToInt((WorldPos.Y / CubeSize) + 1),
        FMath::RoundToInt((WorldPos.Z / CubeSize) + 1)
    );
}

void ARubiksCube::StartRotation(FIntVector LayerCenter, bool bClockwise)
{
   /* if (bIsRotating) return;
    CalculateRotationParams(LayerCenter, bClockwise);
    bIsRotating = true;*/

    if (bIsRotating) return;

    // 重置旋转状态
    CurrentRotation = FRotationData();
    CalculateRotationParams(LayerCenter, bClockwise);

    // 立即完成模式（用于打乱）
    if (!IsInGameThread() || GetWorld()->IsPreviewWorld()) {
        CurrentRotation.CurrentAngle = CurrentRotation.TargetAngle;
        FinalizeRotation();
    }
    else {
        bIsRotating = true;
    }
}

void ARubiksCube::CalculateRotationParams(FIntVector LayerCenter, bool bClockwise)
{
    CurrentRotation = FRotationData();

    // 计算轴心世界坐标
    CurrentRotation.PivotWorld = LogicalToWorld(LayerCenter);

    // 确定旋转轴
    if (LayerCenter.X == 0 || LayerCenter.X == 2) {
        CurrentRotation.RotationAxis = FVector::XAxisVector;
    }
    else if (LayerCenter.Y == 0 || LayerCenter.Y == 2) {
        CurrentRotation.RotationAxis = FVector::YAxisVector;
    }
    else if (LayerCenter.Z == 0 || LayerCenter.Z == 2) {
        CurrentRotation.RotationAxis = FVector::ZAxisVector;
    }

    // 设置旋转角度（右手法则）
    float Angle = bClockwise ? -90.0f : 90.0f;
    CurrentRotation.TargetAngle = Angle;

    // 筛选受影响立方体
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

    // 判断是否在目标层
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

    // 计算旋转增量（保持世界坐标系旋转）
    const float RotationSpeed = 180.0f;
    float DeltaAngle = FMath::Clamp(
        RotationSpeed * DeltaTime,
        0.0f,
        FMath::Abs(CurrentRotation.TargetAngle - CurrentRotation.CurrentAngle)
    ) * FMath::Sign(CurrentRotation.TargetAngle);

    // 创建世界空间旋转四元数
    FQuat WorldRotationDelta = FQuat(CurrentRotation.RotationAxis,
        FMath::DegreesToRadians(DeltaAngle));

    // 应用旋转到每个受影响立方体
    for (int32 Index : CurrentRotation.AffectedIndices) {
        FCubeData& Cube = Cubes[Index];

        // 计算相对轴心的位置
        FVector RelativePos = Cube.Mesh->GetComponentLocation() - CurrentRotation.PivotWorld;

        // 应用世界空间旋转（保持旋转轴始终为世界坐标系方向）
        FVector NewRelativePos = WorldRotationDelta.RotateVector(RelativePos);
        Cube.Mesh->SetWorldLocation(CurrentRotation.PivotWorld + NewRelativePos);

        // 更新旋转（保持世界空间旋转累积）
        FQuat NewWorldRotation = WorldRotationDelta * Cube.Mesh->GetComponentQuat();
        Cube.Mesh->SetWorldRotation(NewWorldRotation);
    }

    CurrentRotation.CurrentAngle += DeltaAngle;

    // 完成旋转处理
    if (FMath::Abs(CurrentRotation.CurrentAngle) >= FMath::Abs(CurrentRotation.TargetAngle)) {
        FinalizeRotation();
        bIsRotating = false;
    }

    // 绘制当前旋转轴
    DrawDebugLine(GetWorld(),
        CurrentRotation.PivotWorld,
        CurrentRotation.PivotWorld + CurrentRotation.RotationAxis * 200,
        FColor::Green,
        false, -1, 0, 2.0f);

    // 显示立方体旋转角度
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
    // 更新逻辑坐标
    for (int32 Index : CurrentRotation.AffectedIndices) {
        FCubeData& Cube = Cubes[Index];
        Cube.LogicalPos = WorldToLogical(Cube.Mesh->GetComponentLocation());
    }

    // 强制对齐到网格并标准化旋转
    for (FCubeData& Cube : Cubes) {
        // 对齐位置
        FVector AlignedPos = LogicalToWorld(Cube.LogicalPos);
        Cube.Mesh->SetWorldLocation(AlignedPos);

        // 标准化旋转（分解为90度整数倍）
        FRotator NormalizedRot = Cube.Mesh->GetComponentRotation().GetNormalized();
        NormalizedRot.Pitch = FMath::GridSnap(NormalizedRot.Pitch, 90.0f);
        NormalizedRot.Yaw = FMath::GridSnap(NormalizedRot.Yaw, 90.0f);
        NormalizedRot.Roll = FMath::GridSnap(NormalizedRot.Roll, 90.0f);
        Cube.Mesh->SetWorldRotation(NormalizedRot);
        Cube.CurrentRotation = NormalizedRot;
    }
    // 新增隐藏逻辑
    if (CurrentRotation.AffectedIndices.Num() > 0)
    {
       //HideIdenticalRotatedCubes(CurrentRotation.LayerCenter);
    }

    // 验证代码（开发时使用）
#if WITH_EDITOR
    for (const FCubeData& Cube : Cubes) {
        FVector ActualPos = Cube.Mesh->GetComponentLocation();
        FIntVector CalcLogical = WorldToLogical(ActualPos);

        // 确保坐标在0-2范围内
        checkf(CalcLogical.X >= 0 && CalcLogical.X <= 2,
            TEXT("Invalid logical X position!"));
        checkf(CalcLogical.Y >= 0 && CalcLogical.Y <= 2,
            TEXT("Invalid logical Y position!"));
        checkf(CalcLogical.Z >= 0 && CalcLogical.Z <= 2,
            TEXT("Invalid logical Z position!"));
    }
#endif
}

// 新增函数实现
TArray<FIntVector> ARubiksCube::GetAllPossibleLayers() const
{
    // 所有可旋转的层中心坐标
    return {
        FIntVector(0,1,1), // 左层
        FIntVector(2,1,1), // 右层
        FIntVector(1,0,1), // 底层
        FIntVector(1,2,1), // 顶层
        FIntVector(1,1,0), // 前层
        FIntVector(1,1,2)  // 后层
    };
}

TArray<TTuple<FIntVector, bool>> ARubiksCube::GenerateScrambleSequence(int32 NumMoves)
{
    TArray<TTuple<FIntVector, bool>> Sequence;
    const TArray<FIntVector> PossibleLayers = GetAllPossibleLayers();
    FIntVector LastLayer = FIntVector::NoneValue;

    for (int32 i = 0; i < NumMoves; i++) {
        // 随机选择层（避免连续相同层）
        FIntVector SelectedLayer;
        do {
            SelectedLayer = PossibleLayers[FMath::RandRange(0, PossibleLayers.Num() - 1)];
        } while (SelectedLayer == LastLayer);

        // 随机旋转方向
        bool bClockwise = FMath::RandBool();

        // 避免无意义旋转组合（如连续相反方向旋转同一层）
        if (i > 0 && Sequence.Last().Get<0>() == SelectedLayer) {
            bClockwise = !Sequence.Last().Get<1>();
        }
        else {
            LastLayer = SelectedLayer;
            Sequence.Emplace(SelectedLayer, bClockwise);
        }
    }
    return Sequence;
}

void ARubiksCube::ScrambleCube(int32 ScrambleMoves, bool bWithAnimation)
{
    const TArray<TTuple<FIntVector, bool>> ScrambleSequence = GenerateScrambleSequence(ScrambleMoves);

    for (const auto& Move : ScrambleSequence) {
        StartRotation(Move.Get<0>(), Move.Get<1>());
        if (bWithAnimation) {
            // 等待动画完成
            while (bIsRotating) {
                Tick(0.016f); // 模拟帧更新
            }
        }
        else {
            // 立即完成
            CurrentRotation.CurrentAngle = CurrentRotation.TargetAngle;
            FinalizeRotation();
        }
    }
}

int32 ARubiksCube::HideIdenticalRotatedCubes(FIntVector LayerCenter)
{
    // 步骤1：获取所有位于相邻层的唯一立方体索引
    TArray<FIntVector> AdjacentLayers = GetAdjacentLayer(LayerCenter);
    TSet<int32> UniqueIndices;

    // lambda：判断某个逻辑坐标是否匹配任一相邻层
    auto MatchesAdjacentLayer = [&](const FIntVector& CubePos) -> bool {
        for (const FIntVector& AdjLayer : AdjacentLayers)
        {
            if ((AdjLayer.X != -1 && CubePos.X == AdjLayer.X) ||
                (AdjLayer.Y != -1 && CubePos.Y == AdjLayer.Y) ||
                (AdjLayer.Z != -1 && CubePos.Z == AdjLayer.Z))
            {
                return true;
            }
        }
        return false;
        };

    for (int32 i = 0; i < Cubes.Num(); i++)
    {
        if (MatchesAdjacentLayer(Cubes[i].LogicalPos))
        {
            UniqueIndices.Add(i);
        }
    }

    // 步骤2：遍历唯一索引集合，对每个连通区域只隐藏一次
    TSet<int32> ProcessedIndices;
    int32 TotalHiddenCount = 0;
    TArray<int32> UniqueIndicesArray = UniqueIndices.Array();

    for (int32 Index : UniqueIndicesArray)
    {
        if (ProcessedIndices.Contains(Index))
        {
            continue;
        }

        // 查找与该索引连通且旋转一致的区域
        TArray<int32> Connected = FindConnectedCubes(Index, UniqueIndicesArray);
        if (Connected.Num() >= 2)
        {
            int32 GroupHiddenCount = 0;
            for (int32 ConnectedIndex : Connected)
            {
                // 仅隐藏当前依然可见的正方体
                if (Cubes[ConnectedIndex].Mesh->IsVisible())
                {
                    Cubes[ConnectedIndex].Mesh->SetVisibility(false);
                    GroupHiddenCount++;
                }
            }
            if (GroupHiddenCount > 0)
            {
                UE_LOG(LogTemp, Log, TEXT("Hidden %d cubes with identical rotations"), GroupHiddenCount);
                TotalHiddenCount += GroupHiddenCount;
            }
            ProcessedIndices.Append(Connected);
        }
    }

    return TotalHiddenCount;
}

TArray<FIntVector> ARubiksCube::GetAdjacentLayer(FIntVector LayerCenter) const
{
    TArray<FIntVector> AdjacentLayers;

    // 根据旋转轴确定相邻层坐标
    if (LayerCenter.X == 0 || LayerCenter.X == 2)
    {
        // 对于X轴层，认为“同Y层”或“同Z层”算作相邻
        AdjacentLayers.Add(FIntVector(-1, LayerCenter.Y, -1)); // 表示匹配 Y 坐标
        AdjacentLayers.Add(FIntVector(-1, -1, LayerCenter.Z)); // 表示匹配 Z 坐标
    }
    else if (LayerCenter.Y == 0 || LayerCenter.Y == 2)
    {
        AdjacentLayers.Add(FIntVector(LayerCenter.X, -1, -1)); // 匹配 X 坐标
        AdjacentLayers.Add(FIntVector(-1, -1, LayerCenter.Z)); // 匹配 Z 坐标
    }
    else if (LayerCenter.Z == 0 || LayerCenter.Z == 2)
    {
        AdjacentLayers.Add(FIntVector(LayerCenter.X, -1, -1)); // 匹配 X 坐标
        AdjacentLayers.Add(FIntVector(-1, LayerCenter.Y, -1)); // 匹配 Y 坐标
    }

    // 过滤掉无效的全部为 -1 的值（理论上不会出现）
    AdjacentLayers.RemoveAll([](const FIntVector& Vec) {
        return Vec.X == -1 && Vec.Y == -1 && Vec.Z == -1;
        });

    return AdjacentLayers;
}

TArray<int32> ARubiksCube::FindConnectedCubes(int32 StartIndex, const TArray<int32>& LayerIndices) const
{
    TArray<int32> Connected;
    TArray<int32> ToCheck;
    const FRotator BaseRotation = Cubes[StartIndex].CurrentRotation;

    ToCheck.Add(StartIndex);

    // 洪水填充算法：遍历所有相邻且旋转一致的立方体
    while (ToCheck.Num() > 0)
    {
        int32 CurrentIndex = ToCheck.Pop();
        if (Connected.Contains(CurrentIndex))
        {
            continue;
        }

        Connected.Add(CurrentIndex);
        FIntVector CurrentPos = Cubes[CurrentIndex].LogicalPos;

        // 定义6个相邻方向
        const TArray<FIntVector> Directions = {
            FIntVector(1, 0, 0),
            FIntVector(-1, 0, 0),
            FIntVector(0, 1, 0),
            FIntVector(0, -1, 0),
            FIntVector(0, 0, 1),
            FIntVector(0, 0, -1)
        };

        for (const FIntVector& Dir : Directions)
        {
            FIntVector NeighborPos = CurrentPos + Dir;
            // 在传入的 LayerIndices 内查找满足逻辑坐标、旋转一致且未处理的正方体
            for (int32 OtherIndex : LayerIndices)
            {
                if (Cubes[OtherIndex].LogicalPos == NeighborPos &&
                    Cubes[OtherIndex].CurrentRotation.Equals(BaseRotation) &&
                    !Connected.Contains(OtherIndex))
                {
                    ToCheck.Add(OtherIndex);
                }
            }
        }
        // 可选：调试绘制当前处理的正方体位置
        DrawDebugBox(GetWorld(),
            Cubes[CurrentIndex].Mesh->GetComponentLocation(),
            FVector(50),
            FColor::Red,
            false,
            2.0f);
    }

    return Connected;
}
