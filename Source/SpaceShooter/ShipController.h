// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipController.generated.h"

UCLASS()
class SPACESHOOTER_API AShipController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShipController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UShapeComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
		float Speed=10.f;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABulletController> BulletBlueprint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABulletsLeft> BulletLeftBlueprint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABulletsRight> BulletRightBlueprint;

	UPROPERTY(EditAnywhere)
		USoundBase* LaserSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ExplodeSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* EmitterShipBP;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void OnShoot();
	void OnRestart();


	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void Teleport();
	void QuitGame();

	UFUNCTION(BlueprintCallable, Category = "ShootLeft")
	void ShootLeft();

	UFUNCTION(BlueprintCallable, Category = "ShootRight")
	void ShootRight();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet")
		ABulletController* Bullet;

	FVector CurrentVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool Died;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	FTimerHandle Timer;
};
