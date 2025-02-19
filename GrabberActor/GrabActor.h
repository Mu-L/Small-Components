#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include"PhysicsEngine/PhysicsConstraintComponent.h"
#include "Camera/CameraComponent.h"
#include "GrabActor.generated.h"

/** Please add a class description */
UCLASS(Blueprintable, BlueprintType)
class AGrabActor : public ACharacter
{
	GENERATED_BODY()
public:
    AGrabActor();
protected:
	//��������
	void DropObject();

	//ɨ��ģ���������
	bool ScanSimulatedPhysicalObject(FHitResult& OutHit);

	//ץס����
	void CaptureTheTarget(FHitResult OutHit);

	//�ƿ�����
	void PushAwayTheObject(FHitResult Hit);

public:
	//ץȡ���ɿ�����
	UFUNCTION(BlueprintCallable)
	void CaptureOrDropObject();

	//�ƿ����壨�������������ץȡ����Ͳ�����ץȡ���壩
    UFUNCTION(BlueprintCallable)
	void PushAwayOrCaptureObject();
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UArrowComponent> Arrow1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> GrabBall;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;

	//���������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	double MouseSensitivity;

	//ץȡ������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TWeakObjectPtr<UPrimitiveComponent> CapturedObject;

	//���ȼ�ʱ
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	bool IntensityTiming;

	//��Զ��״̬
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	bool TelescopeStatus;

	//�ƿ���������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	int32 ForceToPush=1000;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	TArray<AActor*> IgnoreActors;
};
