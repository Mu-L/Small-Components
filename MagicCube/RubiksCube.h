#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RubiksCube.generated.h"

UCLASS()
class TEST_API ARubiksCube : public AActor
{
    GENERATED_BODY()

public:
    ARubiksCube();
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void StartRotation(FIntVector LayerCenter, bool bClockwise);

protected:
    virtual void BeginPlay() override;

private:
    const int32 CubeSize = 100; // 单位：厘米
    UStaticMesh* CubeMesh;

    // 立方体数据结构
    struct FCubeData {
        UStaticMeshComponent* Mesh;
        FIntVector LogicalPos;
        FRotator CurrentRotation;
    };

    TArray<FCubeData> Cubes;

    // 旋转系统参数
    struct FRotationData {
        FVector RotationAxis;
        FVector PivotWorld;
        TArray<int32> AffectedIndices;
        float TargetAngle;
        float CurrentAngle = 0;
    };

    FRotationData CurrentRotation;
    bool bIsRotating = false;

    void InitializeCubes();
    FVector LogicalToWorld(FIntVector LogicalPos) const;
    FIntVector WorldToLogical(FVector WorldPos) const;
    void CalculateRotationParams(FIntVector LayerCenter, bool bClockwise);
    bool IsInRotationLayer(const FVector& CubeWorldPos, FIntVector LayerCenter) const;
    void FinalizeRotation();

private:
    // 打乱相关
    UFUNCTION(BlueprintCallable)
    void ScrambleCube(int32 ScrambleMoves = 25, bool bWithAnimation = true);
    TArray<TTuple<FIntVector, bool>> GenerateScrambleSequence(int32 NumMoves);
    TArray<FIntVector> GetAllPossibleLayers() const;
};