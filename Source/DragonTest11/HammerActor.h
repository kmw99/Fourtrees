#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HammerActor.generated.h"

// 이건 공중에서 왕복 이동하는 망치 액터
// 충돌 박스도 있고, Mesh도 따로 붙어있음
// TargetPoint 2개 사이를 왔다갔다 하게 될 예정

UCLASS()
class DRAGONTEST11_API AHammerActor : public AActor
{
	GENERATED_BODY()

public:
	AHammerActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 충돌 감지용 박스 콜라이더임
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	// 망치 메시 (지금은 큐브 하나 붙일 예정)
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	// 이동할 지점 두 개. 시작점과 도착점
	UPROPERTY(EditAnywhere)
	AActor* PointA;

	UPROPERTY(EditAnywhere)
	AActor* PointB;

	// 이동 속도
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 2000.0f;

private:
	// 현재 목표 지점 (PointA 또는 PointB 중 하나일 예정)
	AActor* CurrentTargetActor;

	// 도착하면 목표를 바꾸는 함수
	void SwitchTarget();

	// 충돌 이벤트 발생 시 호출될 함수
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
