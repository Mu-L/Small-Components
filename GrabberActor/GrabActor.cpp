#include"GrabActor.h"
#include "Kismet/GameplayStatics.h"

AGrabActor::AGrabActor()
{

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);
    GrabBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabBall"));
    GrabBall->SetupAttachment(RootComponent);
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
    PhysicsConstraint->SetupAttachment(GrabBall);
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	// 初始化忽略自身
	IgnoreActors.Add(this);
}

void AGrabActor::DropObject()
{
	PhysicsConstraint->BreakConstraint();
	CapturedObject->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

bool AGrabActor::ScanSimulatedPhysicalObject(FHitResult& OutHit)
{
	FVector Start=Camera->GetComponentLocation();
	//FVector Start = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector End=Camera->GetForwardVector()*10000.0f+Start;
	//FVector End = Start * 10000.0f + Start;

	FHitResult HitResult;
	bool bIsHit=UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::Persistent,//持续开启
		HitResult,
		true,
		FLinearColor(0, 0, 1), // TraceColor，例如蓝色
		FLinearColor(1, 0, 0), // TraceHitColor，例如红色
		5.0f);
    if (bIsHit)
	{
		OutHit = HitResult;
	}
	return bIsHit;
}

void AGrabActor::CaptureTheTarget(FHitResult OutHit)
{
	GrabBall->SetWorldLocation(OutHit.Location, false,nullptr, TeleportFlagToEnum(true));
	//设置约束组件
	PhysicsConstraint->SetConstrainedComponents(GrabBall, FName("None"),Cast<UPrimitiveComponent>( OutHit.Component), OutHit.BoneName);
	//将碰撞响应设置为通道
	OutHit.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CapturedObject = OutHit.Component;
}

void AGrabActor::PushAwayTheObject(FHitResult Hit)
{
	FVector Impulse=Camera->GetForwardVector()*ForceToPush;
	Hit.Component->AddVelocityChangeImpulseAtLocation(Impulse, Hit.Location, Hit.BoneName);
}

void AGrabActor::CaptureOrDropObject()
{
	bool bIsCaptureObject= UKismetSystemLibrary::IsValid(Cast<const UObject>(CapturedObject));
	if (bIsCaptureObject)
	{
		DropObject();
		CapturedObject=nullptr;
	}
	else
	{
		FHitResult OutHit;
		bool bIsScanSimulatedPhysicalObject =ScanSimulatedPhysicalObject(OutHit);
		if (bIsScanSimulatedPhysicalObject)
		{
            CaptureTheTarget(OutHit);
		}
	}
}

void AGrabActor::PushAwayOrCaptureObject()
{
	bool bIsCaptureObject = UKismetSystemLibrary::IsValid(Cast<const UObject>(CapturedObject));
	if (bIsCaptureObject)
	{
		DropObject();
		FHitResult OutHit;
		OutHit.Location=CapturedObject->GetComponentLocation();
		OutHit.Component=CapturedObject;
		PushAwayTheObject(OutHit);
		CapturedObject=nullptr;
	}
	else 
	{
		FHitResult OutHit;
		bool bIsScanSimulatedPhysicalObject = ScanSimulatedPhysicalObject(OutHit);
		if (bIsScanSimulatedPhysicalObject)
		{
			PushAwayTheObject(OutHit);
		}
	}
}
