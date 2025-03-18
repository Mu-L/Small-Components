#include "Truck.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"

ATruck::ATruck()
{
    PrimaryActorTick.bCanEverTick = true;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATruck::HandleBeginOverlap);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATruck::HandleEndOverlap);
}

void ATruck::BeginPlay()
{
    Super::BeginPlay();
    PreviousLocation = GetActorLocation();
}

void ATruck::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���㱾֡λ������
    const FVector CurrentLocation = GetActorLocation();
    const FVector DeltaMovement = CurrentLocation - PreviousLocation;
    PreviousLocation = CurrentLocation;

    // �ֶ�ͬ���˿�λ��
    for (auto& PassengerPtr : Passengers)
    {
        if (APawn* Passenger = PassengerPtr.Get())
        {
            if (IsValid(Passenger))
            {
                Passenger->SetActorLocation(
                    Passenger->GetActorLocation() + DeltaMovement,
                    false, nullptr, ETeleportType::TeleportPhysics
                );
            }
        }
    }

    // �����ƶ��߼�
    if (bIsMoving)
    {
        const float SplineLength = SplineComponent->GetSplineLength();
        const float RemainingDistance = SplineLength - CurrentDistance;

        // �յ㾫ȷֹͣ�߼�
        if (RemainingDistance <= StopTolerance && !bShouldLoop)
        {
            CurrentDistance = SplineLength;
            bIsMoving = false;
            SetActorLocation(SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World));
            return;
        }

        // ������Ч�ƶ�����
        const float EffectiveDelta = bShouldLoop ?
            MovementSpeed * DeltaTime :
            FMath::Min(MovementSpeed * DeltaTime, RemainingDistance);

        CurrentDistance += EffectiveDelta;

        // ѭ������
        if (bShouldLoop && CurrentDistance > SplineLength)
        {
            CurrentDistance = FMath::Fmod(CurrentDistance, SplineLength);
        }

        // ���¿���λ��
        const FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
            CurrentDistance,
            ESplineCoordinateSpace::World
        );
        SetActorLocation(NewLocation);
    }
}

void ATruck::StartMoving()
{
    bIsMoving = true;
}

void ATruck::PauseMoving()
{
    bIsMoving = false;
}

void ATruck::SetShouldLoop(bool bLoop)
{
    bShouldLoop = bLoop;
}

void ATruck::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APawn* Pawn = Cast<APawn>(OtherActor))
    {
        if (!Passengers.Contains(Pawn))
        {
            Passengers.Add(Pawn);
            PreviousLocation = GetActorLocation(); // ��ʼ��λ�û�׼

            // ��ѡ�����ý�ɫ����
            if (UCharacterMovementComponent* Movement = Pawn->FindComponentByClass<UCharacterMovementComponent>())
            {
                Movement->SetMovementMode(MOVE_Flying);
            }
        }
    }
}

void ATruck::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APawn* Pawn = Cast<APawn>(OtherActor))
    {
        Passengers.Remove(Pawn);

        // �ָ���ɫ����״̬
        if (UCharacterMovementComponent* Movement = Pawn->FindComponentByClass<UCharacterMovementComponent>())
        {
            Movement->SetMovementMode(MOVE_Walking);
            Movement->Velocity += GetVelocity(); // �̳п����ٶ�
        }
    }
}