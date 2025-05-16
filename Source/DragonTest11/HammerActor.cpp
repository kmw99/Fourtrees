#include "HammerActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

AHammerActor::AHammerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜라이더 생성. 충돌 감지를 위해서 사용함
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(50, 50, 50)); // 박스 크기
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 겹침만 감지할 예정

	// 메시 설정. 지금은 큐브 하나 붙일 예정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// 나중에 충돌한 액터에서 태그로 구분할 수 있도록 "Hammer" 태그 추가
	Tags.Add("Hammer");
}

void AHammerActor::BeginPlay()
{
	Super::BeginPlay();

	// 시작할 때 PointA 위치로 이동하고, 목표는 PointB로 정해둠
	if (PointA && PointB)
	{
		SetActorLocation(PointA->GetActorLocation());
		CurrentTargetActor = PointB;
	}

	// 충돌 이벤트 연결. 박스가 겹치면 OnOverlap이 호출될 예정
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHammerActor::OnOverlap);
}

void AHammerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PointA || !PointB || !CurrentTargetActor) return;

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = CurrentTargetActor->GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// 실제 이동
	FVector MovementThisFrame = Direction * MoveSpeed * DeltaTime;
	FVector NewLocation = CurrentLocation + MovementThisFrame;
	SetActorLocation(NewLocation);

	// 도착 여부를 DotProduct로 판단
	FVector ToTarget = TargetLocation - NewLocation;
	if (FVector::DotProduct(ToTarget, MovementThisFrame) <= 0.f)
	{
		SwitchTarget();
	}
}


void AHammerActor::SwitchTarget()
{
	// 목표가 PointA면 PointB로 바꾸고, 반대면 다시 PointA로
	if (CurrentTargetActor == PointA)
	{
		CurrentTargetActor = PointB;
	}
	else
	{
		CurrentTargetActor = PointA;
	}
}

void AHammerActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 지금은 단순 로그만 출력하고 있음. 나중엔 드래곤 쪽에서 반응하도록 연결할 예정
	UE_LOG(LogTemp, Warning, TEXT("Hammer overlapped with: %s"), *OtherActor->GetName());
}
