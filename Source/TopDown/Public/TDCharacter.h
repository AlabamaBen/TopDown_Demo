// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundBase.h"
#include "TDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent; 
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UCameraShake;



UCLASS()
class TOPDOWN_API ATDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATDCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShake> ShootCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShake> DamagedCameraShake;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wHUD;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class USkeletalMeshComponent* SkeletonComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	USoundBase* Hurt_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float Max_Life = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float Min_Life = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Gun_Recall = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UParticleSystem* HitPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UParticleSystem* ShootPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UParticleSystemComponent* TrailPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* GunShoot_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* Overheat_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Gun_Cooldown = 0.2f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Max_HeatValue = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UStaticMeshComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class USceneComponent* GunEndComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void MoveForward(float Value);
	void MoveRight(float Value);

	//React to fire input
	void FireInput(float Value);
	//Take one shot
	void Fire();

public:	
	virtual void Tick(float DeltaTime) override;

	//Setting up input for controller
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Get damage and apply camera shake
	void GetDamage(float value);

	// Initial Skeleton Rotation to add when looking at mouse
	float Init_Skeleton_Yaw = 0; 

	bool IsDead = false;


private: 

	float GetLifePercent();
	UUserWidget* HUD_Inst;
	float Current_Life = 100;
	float Current_HeatValue = 0.0f;
	bool has_overheated = false;
	float Gun_Cooldown_value = 0.0f;
	int Score = 0;

};