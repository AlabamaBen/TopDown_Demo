// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "TDEnemy.generated.h"


UCLASS()
class TOPDOWN_API ATDEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATDEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		class USkeletalMeshComponent* SkeletonComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		class UParticleSystem* DiePS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class UStaticMeshComponent* WeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DiePS_Size = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		USoundBase* Die_Sound;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initial Skeleton Rotation to compensate direction
	float Init_Skeleton_Yaw = 0;

	// Make explosion and destroy
	void Die();

private: 
	bool dying = false; 
};
