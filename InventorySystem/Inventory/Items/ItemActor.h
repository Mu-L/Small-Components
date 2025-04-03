#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class UItemObject;
class USphereComponent;

UCLASS()
class  AItemActor : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 dimensionX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 dimensionY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* InIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* InRIcon;

	//用于丢弃的物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> itemClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadWrite)
	UItemObject* ItemObject;

	//UFUNCTION(BlueprintImplementableEvent)
	virtual UItemObject* GetDefaultItemObject();
};
