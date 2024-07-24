// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletsLeft.h"
#include "Components/BoxComponent.h"

// Sets default values
ABulletsLeft::ABulletsLeft()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void ABulletsLeft::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletsLeft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.Y -= Speed * DeltaTime;

	SetActorLocation(NewLocation);

	if (NewLocation.Y < -1700.0f) { this->Destroy(); }
}

