#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "DragonRandomFlyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DRAGONTEST11_API UDragonRandomFlyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDragonRandomFlyComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying", meta = (AllowPrivateAccess = "true"))
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
	float ArrivalDistance = 100.0f;

	// Spline 관련 속성 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent* FollowSpline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	int32 NumSplinePoints = 40;

private:
	AActor* Owner;
	FVector CurrentTarget;

	void MoveToTarget(float DeltaTime);
	void PickNewTarget();
	void UpdateSplineFromBones();
};
