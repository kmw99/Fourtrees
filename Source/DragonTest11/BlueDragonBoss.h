#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueDragonBoss.generated.h"

class USkeletalMeshComponent;
class UDragonRandomFlyComponent;

UCLASS()
class DRAGONTEST11_API ABlueDragonBoss : public AActor
{
	GENERATED_BODY()

public:
	ABlueDragonBoss();

protected:
	virtual void BeginPlay() override;

	// 메시 충돌 발생 시 실행될 함수 선언
	UFUNCTION()
	void OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* DragonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDragonRandomFlyComponent* FlyComponent;
};
