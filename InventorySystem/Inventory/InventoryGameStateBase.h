#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InventoryGameStateBase.generated.h"

class UItemObject;
/**
 *
 */
UCLASS()
class  AInventoryGameStateBase : public AGameStateBase
{
    GENERATED_BODY()
public:
    /**
     * 从指定Actor的位置生成物品
     * @param Actor - 源Actor（用于获取生成位置和朝向）
     * @param ItemObject - 物品数据对象（包含要生成的物品类信息）
     * @param GroundClamp - 是否将物品对齐到地面（默认true）
     */
    void SpawnItemFromActor(AActor* Actor, UItemObject* ItemObject, bool GroundClamp = true) const;
};