// Fill out your copyright notice in the Description page of Project Settings.


#include "TDEnemy_Spawner.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "TDEnemy.h"
#include "TDEnemy_AIController.h"


// Sets default values
ATDEnemy_Spawner::ATDEnemy_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATDEnemy_Spawner::BeginPlay()
{
	Super::BeginPlay();

	//Create custom tick 
	GetWorld()->GetTimerManager().SetTimer(SpawnTickTimer, this, &ATDEnemy_Spawner::TrySpawn, Spawning_Rate, true, Spawning_Rate);;
}

// Called every frame
void ATDEnemy_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATDEnemy_Spawner::TrySpawn()
{
	FActorSpawnParameters SpawnParams; 

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 

	ATDEnemy* SpawnerActorRef = GetWorld()->SpawnActor<ATDEnemy>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

}