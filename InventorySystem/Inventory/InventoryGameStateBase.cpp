#include "InventoryGameStateBase.h"
#include "GameFramework/Character.h"   // ����
#include "Components/CapsuleComponent.h" // ���������ڻ�ȡ��ɫ�����壩
#include "GameFramework/PlayerController.h" // ���������ڻ�ȡ��������ת��
#include "Items/ItemActor.h"
#include "Items/ItemObject.h"

void AInventoryGameStateBase::SpawnItemFromActor(AActor* Actor, UItemObject* ItemObject, bool GroundClamp) const
{
    // �����ʼ����λ�ã�Actorǰ��150��λ��
    FVector SpawnLocation = Actor->GetActorLocation() + Actor->GetActorForwardVector() * 150.f;

    if (GroundClamp)
    {
        // ��������߼�
        FHitResult HitResult;
        FVector Start = SpawnLocation;                     // �������
        FVector End = SpawnLocation - FVector::UpVector * 1000.f; // ����1000��λ���յ�
        FCollisionQueryParams CollisionQueryParams;
        CollisionQueryParams.AddIgnoredActor(Actor);       // ����ԴActor����ײ

        // ִ�����߼�⣨ʹ�ÿɼ���ͨ����
        if (GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECollisionChannel::ECC_Visibility,
            CollisionQueryParams))
        {
            // �����⵽���棬������λ������Ϊ��ײ��
            SpawnLocation = HitResult.Location;
        }
    }

    // ��ȡ��Ϸ���粢������Ʒ
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        // ������ײ���������ܵ���λ�õ���֤����
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // ������λ�����50��λ�����⴩͸����/��Ӹ���Ч����
        SpawnLocation.Z += 50.f;

        // ʵ��������ƷActor
        World->SpawnActor<AItemActor>(
            ItemObject->GetItemClass(),    // ��ȡ��Ʒ��ͼ��
            SpawnLocation,                 // ��������λ��
            FRotator::ZeroRotator,         // ����ת��ʹ��Ĭ�ϳ���
            SpawnParams                    // ���ɲ���
        );
    }
}