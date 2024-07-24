// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletsRight.h"
#include "Components/BoxComponent.h"

// Sets default values
ABulletsRight::ABulletsRight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void ABulletsRight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletsRight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += Speed * DeltaTime;

	SetActorLocation(NewLocation);

	if (NewLocation.Y > 1800.0f) { this->Destroy(); }
}

