#include "ItemActor.h"

#include "Components/SphereComponent.h"
#include "InventoryComponent.h"
#include"ItemObject.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);
	//SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (ItemObject == nullptr)
	{
		ItemObject = GetDefaultItemObject();
	}
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATP_ThirdPersonCharacter* Player = Cast<ATP_ThirdPersonCharacter>(OtherActor))
	{
		if (Player->GetInventoryComponent()->TryAddItem(ItemObject))
		{
			Destroy();
		}
	}
}

UItemObject* AItemActor::GetDefaultItemObject()
{
    // 提供基础实现（蓝图可选择覆盖）
	UItemObject* DefaultObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
    if(DefaultObject)
    {
        DefaultObject->InitializeItemObject(dimensionX, dimensionY, InIcon, InRIcon, itemClass);
    }
    return DefaultObject;
}