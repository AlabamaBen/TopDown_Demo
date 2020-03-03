// Fill out your copyright notice in the Description page of Project Settings.


#include "TDEnemy_AIController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"


void ATDEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Target)
	{
		//Create custome tick 
		GetWorld()->GetTimerManager().SetTimer(GoToTarget_Timer, this, &ATDEnemy_AIController::GoToTarget, 0.2f, true, 0.2f);;
	}
	target_inst = Cast<ATDCharacter>(Target); 

}

void ATDEnemy_AIController::Tick(float deltaseconds)
{
	Super::Tick(deltaseconds);


	if (!enemy_inst)
	{
		enemy_inst=  Cast<ATDEnemy>(GetCharacter());
	}

	if (enemy_inst && Target && target_inst)
	{
		//If close to target, self die 
		if (FVector::Distance(enemy_inst->GetActorLocation(), Target->GetActorLocation()) < 200 && !target_inst->IsDead)
		{
			enemy_inst->Die(); 
		}
	}
}

void ATDEnemy_AIController::GoToTarget()
{
	//If target exist, try going to it with navmesh
	if (Target && target_inst)
	{
		if (!target_inst->IsDead)
		{
			MoveToLocation(Target->GetActorLocation());
		}

	}
}
