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

	// ץȡ���ܽӿ�
	void StartGrappling();
	void StopGrappling();
	void HandleGrapplingPhysics();

	// ���ץȡ����
	UPROPERTY(EditAnywhere, Category = "Grappling")
	float MaxGrappleDistance = 3000.f;

	//ץȡ����ϵ��
	UPROPERTY(EditAnywhere, Category = "Grappling")
	float GrappleForceMultiplier = 250000.f;

	//ʹ�õļ��ͨ��
	UPROPERTY(EditAnywhere, Category = "Grappling")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

private:
	// �������
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY()
	UCameraComponent* FollowCamera;

	UPROPERTY()
	UCableComponent* CableComponent;

	// ץȡ״̬
	FVector_NetQuantize GrapplePoint;
	bool bIsGrappling = false;
};