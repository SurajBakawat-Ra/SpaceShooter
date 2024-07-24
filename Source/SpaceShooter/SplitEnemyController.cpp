// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitEnemyController.h"
#include "Components/BoxComponent.h"
#include "BulletController.h"
#include "BulletsLeft.h"
#include "BulletsRight.h"
#include "EnemyController.h"
#include "SSGameMode.h"

// Sets default values
ASplitEnemyController::ASplitEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBox->SetGenerateOverlapEvents(true);
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &ASplitEnemyController::OnOverlap);
}

// Called when the game starts or when spawned
void ASplitEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplitEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.X += Speed * DeltaTime;

	SetActorLocation(NewLocation);

	if (NewLocation.X < -3000.0f) { this->Destroy(); }
}

void ASplitEnemyController::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABulletController::StaticClass())|| OtherActor->IsA(ABulletsRight::StaticClass())|| OtherActor->IsA(ABulletsLeft::StaticClass()))
	{
		FVector Location = GetActorLocation();
		this->Destroy();
		GetWorld()->SpawnActor<AEnemyController>(EnemyBlueprint, Location, FRotator::ZeroRotator);
		GetWorld()->SpawnActor<AEnemyController>(EnemyBlueprint, Location + FVector(0.0f,300.0f,0.0f), FRotator::ZeroRotator);
		GetWorld()->SpawnActor<AEnemyController>(EnemyBlueprint, Location + FVector(0.0f,-300.0f, 0.0f), FRotator::ZeroRotator);

		((ASSGameMode*)GetWorld()->GetAuthGameMode())->IncrementScore();
	}
}