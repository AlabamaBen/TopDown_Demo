// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "TDAnimationInstance.generated.h"

/**
 * 
 */

UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class UTDAnimationInstance: public UAnimInstance
{
	GENERATED_BODY()
public:
	UTDAnimationInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsDead;


	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
