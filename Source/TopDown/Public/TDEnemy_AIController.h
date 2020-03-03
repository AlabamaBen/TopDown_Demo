// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "TDEnemy.h"
#include "TDCharacter.h"
#include "TDEnemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API ATDEnemy_AIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float deltaseconds) override;


public: 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	AActor* Target = nullptr;

	void GoToTarget();

	FTimerHandle GoToTarget_Timer ; 

private : 
	ATDEnemy* enemy_inst = nullptr; 
	ATDCharacter* target_inst = nullptr;
	
};
