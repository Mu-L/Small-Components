// GrapplingComponent.cpp
#include "GrapplingComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "CableComponent.h"
#include"Kismet/KismetMathLibrary.h"
#include"Kismet/KismetSystemLibrary.h"

UGrapplingComponent::UGrapplingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ��ȡ��Ҫ���
	AActor* Owner = GetOwner();
	if (Owner)
	{
		CharacterMovement = Owner->FindComponentByClass<UCharacterMovementComponent>();
		FollowCamera = Owner->FindComponentByClass<UCameraComponent>();

		// ��̬�����������
		CableComponent = NewObject<UCableComponent>(Owner);
		CableComponent->SetupAttachment(FollowCamera);
		CableComponent->RegisterComponent();
        CableComponent->SetVisibility(false);
        CableComponent->NumSegments=1;
	}
}

void UGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGrappling)
	{
		HandleGrapplingPhysics();
	}
}

void UGrapplingComponent::StartGrappling()
{
	if (!FollowCamera) return;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * MaxGrappleDistance;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		TraceTypeQuery1, // ����ö��ֵ
		true,
		{ GetOwner() },
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bHit)
	{
		GrapplePoint = HitResult.ImpactPoint;
		bIsGrappling = true;
		CableComponent->SetVisibility(true);
		if (CharacterMovement)
		{
			CharacterMovement->SetMovementMode(MOVE_Flying);
		}
	}
}

void UGrapplingComponent::StopGrappling()
{
	bIsGrappling = false;
	CableComponent->SetVisibility(false);
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(MOVE_Falling);
	}
}

void UGrapplingComponent::HandleGrapplingPhysics()
{
	if (!bIsGrappling || !GetOwner() || !CharacterMovement) return;

	// ���µ��¶˵�
	CableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(
		GetOwner()->GetActorTransform(),
		GrapplePoint
	);

	// ��������
	FVector ToGrapplePoint = (GrapplePoint - GetOwner()->GetActorLocation()).GetSafeNormal();
	FVector ForceDirection = ToGrapplePoint +
		GetOwner()->GetActorRightVector() * CharacterMovement->GetLastInputVector().Y * 0.7f;

	CharacterMovement->AddForce(ForceDirection.GetSafeNormal() * GrappleForceMultiplier);
}