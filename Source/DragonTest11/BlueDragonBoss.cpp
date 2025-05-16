#include "BlueDragonBoss.h"
#include "Components/SkeletalMeshComponent.h"
#include "DragonRandomFlyComponent.h"
#include "Engine/Engine.h"

ABlueDragonBoss::ABlueDragonBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시 설정
	DragonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonMesh"));
	RootComponent = DragonMesh;

	// 비행 컴포넌트 추가
	FlyComponent = CreateDefaultSubobject<UDragonRandomFlyComponent>(TEXT("FlyComponent"));
}

void ABlueDragonBoss::BeginPlay()
{
	Super::BeginPlay();

	if (DragonMesh)
	{
		// 콜리전 프로파일 설정 (겹침 감지용)
		DragonMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DragonMesh->SetCollisionObjectType(ECC_Pawn);
		DragonMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

		// 충돌 이벤트 연결
		DragonMesh->OnComponentBeginOverlap.AddDynamic(this, &ABlueDragonBoss::OnMeshOverlap);
	}
}

// 충돌 처리 함수
void ABlueDragonBoss::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	for (const FName& Tag : OtherActor->Tags)
	{
		// 게임 화면에 충돌한 애 태그 출력
		if (GEngine)
		{
			FString Message = TEXT("Hit! Tag Name: ") + Tag.ToString();
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, Message);
		}
	}
}
