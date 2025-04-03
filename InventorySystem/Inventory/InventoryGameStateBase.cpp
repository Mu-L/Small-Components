#include "InventoryGameStateBase.h"
#include "GameFramework/Character.h"   // 新增
#include "Components/CapsuleComponent.h" // 新增（用于获取角色胶囊体）
#include "GameFramework/PlayerController.h" // 新增（用于获取控制器旋转）
#include "Items/ItemActor.h"
#include "Items/ItemObject.h"

void AInventoryGameStateBase::SpawnItemFromActor(AActor* Actor, UItemObject* ItemObject, bool GroundClamp) const
{
    // 计算初始生成位置：Actor前方150单位处
    FVector SpawnLocation = Actor->GetActorLocation() + Actor->GetActorForwardVector() * 150.f;

    if (GroundClamp)
    {
        // 地面对齐逻辑
        FHitResult HitResult;
        FVector Start = SpawnLocation;                     // 射线起点
        FVector End = SpawnLocation - FVector::UpVector * 1000.f; // 向下1000单位的终点
        FCollisionQueryParams CollisionQueryParams;
        CollisionQueryParams.AddIgnoredActor(Actor);       // 忽略源Actor的碰撞

        // 执行视线检测（使用可见性通道）
        if (GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECollisionChannel::ECC_Visibility,
            CollisionQueryParams))
        {
            // 如果检测到地面，将生成位置设置为碰撞点
            SpawnLocation = HitResult.Location;
        }
    }

    // 获取游戏世界并生成物品
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        // 设置碰撞处理：尽可能调整位置但保证生成
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // 将生成位置提高50单位（避免穿透地面/添加浮空效果）
        SpawnLocation.Z += 50.f;

        // 实际生成物品Actor
        World->SpawnActor<AItemActor>(
            ItemObject->GetItemClass(),    // 获取物品蓝图类
            SpawnLocation,                 // 最终生成位置
            FRotator::ZeroRotator,         // 无旋转（使用默认朝向）
            SpawnParams                    // 生成参数
        );
    }
}