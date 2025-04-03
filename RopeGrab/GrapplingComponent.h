// GrapplingComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrapplingComponent.generated.h"

class UCableComponent;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UGrapplingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrapplingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 抓取功能接口
	void StartGrappling();
	void StopGrappling();
	void HandleGrapplingPhysics();

	// 最大抓取距离
	UPROPERTY(EditAnywhere, Category = "Grappling")
	float MaxGrappleDistance = 3000.f;

	//抓取力度系数
	UPROPERTY(EditAnywhere, Category = "Grappling")
	float GrappleForceMultiplier = 250000.f;

	//使用的检测通道
	UPROPERTY(EditAnywhere, Category = "Grappling")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

private:
	// 依赖组件
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY()
	UCameraComponent* FollowCamera;

	UPROPERTY()
	UCableComponent* CableComponent;

	// 抓取状态
	FVector_NetQuantize GrapplePoint;
	bool bIsGrappling = false;
};