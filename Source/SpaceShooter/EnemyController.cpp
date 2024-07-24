// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BulletController.h"
#include "BulletsLeft.h"
#include "Components/ShapeComponent.h"
#include "BulletsRight.h"
#include "SSGameMode.h"

// Sets default values
AEnemyController::AEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBox->SetGenerateOverlapEvents(true);
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyController::OnOverlap);
}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.X += Speed * DeltaTime;

	SetActorLocation(NewLocation);

	if (NewLocation.X < -3000.0f) { this->Destroy(); }
}

void AEnemyController::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		if (OtherActor->IsA(ABulletController::StaticClass()) || OtherActor->IsA(ABulletsRight::StaticClass()) || OtherActor->IsA(ABulletsLeft::StaticClass()))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), Sound);
			UGameplayStatics::SpawnEmitterAtLocation((ASSGameMode*)GetWorld(), EmitterBP, this->GetActorLocation());
			this->Destroy();
			

			((ASSGameMode*)GetWorld()->GetAuthGameMode())->IncrementScore();
		}
}