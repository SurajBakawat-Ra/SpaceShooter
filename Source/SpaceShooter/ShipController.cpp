// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BulletController.h"
#include "BulletsLeft.h"
#include "BulletsRight.h"
#include "EnemyController.h"
#include "SplitEnemyController.h"
#include "SSGameMode.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
AShipController::AShipController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AShipController::OnOverlap);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AShipController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AShipController::OnShoot, 1.f, false);
}

// Called every frame
void AShipController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + Speed * CurrentVelocity * DeltaTime;

		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void AShipController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//InputComponent->BindAction("ShootLeft",IE_Pressed, this, &AShipController::ShootLeft);
	//InputComponent->BindAction("ShootRight", IE_Pressed, this, &AShipController::ShootRight);
	//InputComponent->BindAction("Teleport", IE_Pressed, this, &AShipController::Teleport);
	InputComponent->BindAction("Quit", IE_Pressed, this, &AShipController::QuitGame);
	InputComponent->BindAction("Restart", IE_Pressed, this, &AShipController::OnRestart).bExecuteWhenPaused = true;

	InputComponent->BindAxis("MoveX", this, &AShipController::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AShipController::Move_YAxis);
}

void AShipController::Move_XAxis(float AxisValue)
{
	if (GetActorLocation().X > 2550.0f) { CurrentVelocity.X = -150.0f; }
	else if (GetActorLocation().X < -2300.0f) { CurrentVelocity.X = 150.0f; }
	else
	{
		CurrentVelocity.X = AxisValue * 150.0f;
	}
}

void AShipController::Move_YAxis(float AxisValue)
{
	if (GetActorLocation().Y > 1400.0f) { CurrentVelocity.Y = -150.0f; }
	else if (GetActorLocation().Y < -1400.0f) { CurrentVelocity.Y = 150.0f; }
	else
	{
		CurrentVelocity.Y = AxisValue * 150.0f;
	}
}

void AShipController::OnShoot()
{
	UWorld* World = GetWorld();

	if (World)
	{
		FVector Location = GetActorLocation();

		UGameplayStatics::PlaySound2D(GetWorld(), LaserSound);
		Bullet = World->SpawnActor<ABulletController>(BulletBlueprint, Location, FRotator::ZeroRotator);

		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AShipController::OnShoot, 2.f, false);
	}
}

void AShipController::ShootLeft()
{
	UWorld* World = GetWorld();

	if (World)
	{
		World->SpawnActor<ABulletsLeft>(BulletLeftBlueprint, Bullet->GetActorLocation(), FRotator::ZeroRotator);
		World->SpawnActor<ABulletsLeft>(BulletLeftBlueprint, Bullet->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
		World->SpawnActor<ABulletsLeft>(BulletLeftBlueprint, Bullet->GetActorLocation() + FVector(-200.0f, 0.0f, 0.0f), FRotator::ZeroRotator);

		Bullet->SetActorLocation(FVector(5000.0f, 0.0f, 0.0f));
		Bullet->Destroy();
	}
}

void AShipController::ShootRight()
{
	UWorld* World = GetWorld();

	if (World)
	{
		World->SpawnActor<ABulletsRight>(BulletRightBlueprint, Bullet->GetActorLocation(), FRotator::ZeroRotator);
		World->SpawnActor<ABulletsRight>(BulletRightBlueprint, Bullet->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
		World->SpawnActor<ABulletsRight>(BulletRightBlueprint, Bullet->GetActorLocation() + FVector(-200.0f, 0.0f, 0.0f), FRotator::ZeroRotator);

		Bullet->SetActorLocation(FVector(5000.0f, 0.0f , 0.0f));
		Bullet->Destroy();
	}
}

void AShipController::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor->IsA(AEnemyController::StaticClass()) || OtherActor->IsA(ASplitEnemyController::StaticClass()))&& !(OtherActor->IsA(ABulletController::StaticClass()) || OtherActor->IsA(ABulletsRight::StaticClass()) || OtherActor->IsA(ABulletsLeft::StaticClass())))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ExplodeSound);
		UGameplayStatics::SpawnEmitterAtLocation((ASSGameMode*)GetWorld(), EmitterShipBP, this->GetActorLocation());
		Died = true;

		this->SetActorHiddenInGame(true);

		((ASSGameMode*)GetWorld()->GetAuthGameMode())->OnGameOver();


		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AShipController::OnRestart()
{
	if (Died)
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}


void AShipController::Teleport()
{
	if (Bullet->GetActorLocation().X <2700.0f)
	{
		SetActorLocation(Bullet->GetActorLocation());
	}
}

void AShipController::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}



