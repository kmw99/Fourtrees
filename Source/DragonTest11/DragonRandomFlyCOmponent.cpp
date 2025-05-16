#include "DragonRandomFlyComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"

UDragonRandomFlyComponent::UDragonRandomFlyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDragonRandomFlyComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	PickNewTarget(); // 시작할 때 한 번 목표를 정함
}

void UDragonRandomFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner || Waypoints.Num() == 0) return;

	MoveToTarget(DeltaTime);

	// Spline 갱신 추가
	UpdateSplineFromBones();
}

void UDragonRandomFlyComponent::MoveToTarget(float DeltaTime)
{
	// SkeletalMeshComponent 가져오기
	USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	// 기준이 되는 본(SOXNFB_Bone002)의 월드 위치를 가져옴
	FVector BoneLocation = Mesh->GetBoneLocation(TEXT("SOXNFB_Bone002"));

	// 이동 방향 계산
	FVector Direction = (CurrentTarget - BoneLocation).GetSafeNormal();

	// 액터를 이동 (본이 아니라 전체 액터를 움직임)
	FVector NewLocation = Owner->GetActorLocation() + Direction * Speed * DeltaTime;
	Owner->SetActorLocation(NewLocation);

	// 부드러운 회전 처리
	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator TargetRot = Direction.Rotation();
	FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 1.5f);
	Owner->SetActorRotation(SmoothRot);

	// 도착 판정: 본이 목표지점에 가까워졌는지 확인
	if (FVector::Dist(BoneLocation, CurrentTarget) < ArrivalDistance)
	{
		PickNewTarget();
	}
}

void UDragonRandomFlyComponent::PickNewTarget()
{
	if (Waypoints.Num() == 0) return;

	int Index = FMath::RandRange(0, Waypoints.Num() - 1);
	CurrentTarget = Waypoints[Index]->GetActorLocation();
}

// 드래곤 본 위치를 기반으로 Spline을 갱신하는 함수
void UDragonRandomFlyComponent::UpdateSplineFromBones()
{
	if (!FollowSpline) return;

	USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	for (int32 i = 0; i < NumSplinePoints; ++i)
	{
		FString BoneName = FString::Printf(TEXT("SOXNFB_Bone%03d"), 2 + i);
		FVector BoneLocation = Mesh->GetBoneLocation(FName(*BoneName));
		FollowSpline->SetLocationAtSplinePoint(i, BoneLocation, ESplineCoordinateSpace::World);
	}

	FollowSpline->UpdateSpline();
}
