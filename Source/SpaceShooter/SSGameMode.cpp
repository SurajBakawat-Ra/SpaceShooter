// Fill out your copyright notice in the Description page of Project Settings.


#include "SSGameMode.h"
#include "EnemyController.h"
#include "SplitEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameWidget.h"

void ASSGameMode::BeginPlay()
{
	Super::BeginPlay();

	ChangeMenuWidget(StartingWidgetClass);

	((UGameWidget*)CurrentWidget)->Load(); 
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void ASSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.0f)
	{
		float DifficultyPercentage = FMath::Min(GameTimer / TIME_TO_MINIMUM_INTERVAL, 1.0f);

		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL)*DifficultyPercentage;

		UWorld* World = GetWorld();
		if (World)
		{
			FVector Location = FVector(2999.f, FMath::RandRange(-1700.0f, 1700.0f), 120.0f);
			FVector Location2 = FVector(2999.f, FMath::RandRange(-1700.0f, 1700.0f), 120.0f);
			World->SpawnActor<AEnemyController>(EnemyBlueprint, Location, FRotator::ZeroRotator);
			World->SpawnActor<ASplitEnemyController>(SplitEnemyBlueprint, Location2, FRotator::ZeroRotator);
		}
	}
}

void ASSGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

}

void ASSGameMode::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void ASSGameMode::OnGameOver()
{
	EndScore = Score;
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);
}