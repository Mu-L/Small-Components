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
	//丢下物体
	void DropObject();

	//扫描模拟物理对象
	bool ScanSimulatedPhysicalObject(FHitResult& OutHit);

	//抓住对象
	void CaptureTheTarget(FHitResult OutHit);

	//推开物体
	void PushAwayTheObject(FHitResult Hit);

public:
	//抓取或松开物体
	UFUNCTION(BlueprintCallable)
	void CaptureOrDropObject();

	//推开物体（两种情况：存在抓取物体和不存在抓取物体）
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

	//鼠标灵敏度
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	double MouseSensitivity;

	//抓取的物体
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TWeakObjectPtr<UPrimitiveComponent> CapturedObject;

	//力度计时
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	bool IntensityTiming;

	//望远镜状态
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	bool TelescopeStatus;

	//推开物体力度
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	int32 ForceToPush=1000;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	TArray<AActor*> IgnoreActors;
};
