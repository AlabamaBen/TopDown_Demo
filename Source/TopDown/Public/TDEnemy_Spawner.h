// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "TDEnemy_Spawner.generated.h"


UCLASS()
class TOPDOWN_API ATDEnemy_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDEnemy_Spawner();
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TrySpawn(); 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float Spawning_Rate = 2.0f;
	FTimerHandle SpawnTickTimer;


};
