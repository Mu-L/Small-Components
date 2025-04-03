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
     * ��ָ��Actor��λ��������Ʒ
     * @param Actor - ԴActor�����ڻ�ȡ����λ�úͳ���
     * @param ItemObject - ��Ʒ���ݶ��󣨰���Ҫ���ɵ���Ʒ����Ϣ��
     * @param GroundClamp - �Ƿ���Ʒ���뵽���棨Ĭ��true��
     */
    void SpawnItemFromActor(AActor* Actor, UItemObject* ItemObject, bool GroundClamp = true) const;
};