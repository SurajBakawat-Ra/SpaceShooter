// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASSGameMode : public AGameMode
{
	GENERATED_BODY()

	float MINIMUM_INTERVAL = 0.5f;
	float MAXIMUM_INTERVAL = 2.0f;
	float TIME_TO_MINIMUM_INTERVAL = 30.f;

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AEnemyController> EnemyBlueprint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASplitEnemyController> SplitEnemyBlueprint;

	float EnemyTimer;
	float GameTimer;

	void IncrementScore();
	UFUNCTION(BlueprintCallable, Category = "EndGame")
	void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	UPROPERTY(BlueprintReadOnly)
	int Score;

	UPROPERTY(BlueprintReadOnly)
		int EndScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere)
		USoundBase* Sound;
};
