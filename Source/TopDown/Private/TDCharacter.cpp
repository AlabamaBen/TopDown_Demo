// Fill out your copyright notice in the Description page of Project Settings.


#include "TDCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "TDEnemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATDCharacter::ATDCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	SkeletonComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	SkeletonComp->SetupAttachment(RootComponent);


	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponComp->SetupAttachment(RootComponent);

	GunEndComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunEnd"));
	GunEndComp->SetupAttachment(WeaponComp);

	TrailPS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail_PS"));
	TrailPS->SetupAttachment(WeaponComp);


}

// Called when the game starts or when spawned
void ATDCharacter::BeginPlay()
{
	Super::BeginPlay();

	Init_Skeleton_Yaw = SkeletonComp->GetComponentRotation().Yaw;

	//Attach gun to skeleton
	const FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true); 
	WeaponComp->AttachToComponent(SkeletonComp, rules, "RGun");

	//Create HUD widget and add it to viewport
	if (wHUD) 
	{
		HUD_Inst = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), wHUD);

		if (HUD_Inst)
		{
			HUD_Inst->AddToViewport();
		}
	}

	Current_Life = Max_Life; 
}

void ATDCharacter::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f) && !IsDead)
	{
		AddMovementInput( FVector::ForwardVector, Value);
	}
	
}

void ATDCharacter::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f) && !IsDead )
	{
		AddMovementInput(FVector::RightVector, Value);
	}
}

void ATDCharacter::FireInput(float Value)
{
	if ((Value != 0.0f) && !IsDead)
	{
		//if gun cooldown is over and gun isnt overheated
		if (Gun_Cooldown_value > Gun_Cooldown && !has_overheated)
		{
			//Refesh cooldown
			Gun_Cooldown_value = 0.0f;
			//Increase heat value
			Current_HeatValue += Gun_Cooldown * 2 ;
			//Fire on shot
			Fire(); 

			//Check overheating
			if (Current_HeatValue > Max_HeatValue)
			{
				Current_HeatValue = Max_HeatValue;
				has_overheated = true; 
				UGameplayStatics::PlaySound2D(GetWorld(), Overheat_Sound);

			}
		}
	}
}

void ATDCharacter::Fire()
{
	FHitResult OutHit; 

	//Make random offset to apply to the gunshot
	float rand_yaw = FMath::FRandRange(-Gun_Recall, Gun_Recall);

	//Raycasting in skeleton direction looking for hit
	FVector Start = GunEndComp->GetComponentLocation(); 
	FVector ForwardVector = FRotator(0, SkeletonComp->GetComponentRotation().Yaw - Init_Skeleton_Yaw + rand_yaw, 0).Vector();
	FVector End = ((ForwardVector * 1000.f) + Start) ;
	FTransform ShootTransform = FTransform(GunEndComp->GetComponentRotation(), Start, FVector(1, 1, 1));
	FCollisionQueryParams CollsionParams; 
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollsionParams);

	if (isHit)
	{
		ATDEnemy* hitEnemy = Cast<ATDEnemy>(OutHit.GetActor());

		//If it's enemy, make it die and increment score
		if (hitEnemy)
		{
			hitEnemy->Die();
			Score++;
		}
		//if it's something else, make a gun impact
		else
		{
			FTransform HitTransform = FTransform(FRotator::ZeroRotator, OutHit.Location, FVector(1, 1, 1));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPS, HitTransform);
		}
	}


	//Apply all effects and sounds
	if (ShootPS)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootPS, ShootTransform);
	}
	if (TrailPS)
	{
		FTransform TrailTransform = GunEndComp->GetComponentTransform(); 
		TrailTransform.SetRotation(FRotator(0, SkeletonComp->GetComponentRotation().Yaw - Init_Skeleton_Yaw + rand_yaw, 0).Quaternion()); 
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailPS->Template, TrailTransform);
	}
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (camManager)
	{
		camManager->PlayCameraShake(ShootCameraShake);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), GunShoot_Sound); 
}

void ATDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Check if dead
	if (Current_Life <= Min_Life)
	{
		Current_Life = Min_Life; 
		IsDead = true; 
	}

	//if not dead
	if (!IsDead)
	{
		//Update gun cooldown and heating
		if (Gun_Cooldown_value < Gun_Cooldown)
		{
			Gun_Cooldown_value += DeltaTime;
		}
		else
		{
			if (Current_HeatValue > 0)
			{
				Current_HeatValue -= DeltaTime;
			}
			if (Current_HeatValue < 0 && has_overheated)
			{
				has_overheated = false; 
				Current_HeatValue = 0;
			}
		}

		//Make skeleton look at mouse position
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr)
		{
			// Get the coordinates of the mouse from our controller  
			float LocationX;
			float LocationY;
			PlayerController->GetMousePosition(LocationX, LocationY);
			// Do a trace and see if there the position intersects something in the world  
			FVector2D MousePosition(LocationX, LocationY);
			FHitResult HitResult;
			const bool bTraceComplex = false;
			if (PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_WorldStatic, bTraceComplex, HitResult) == true)
			{
			
				FVector LookVtc = HitResult.Location - GetActorLocation();
				LookVtc.Z = 0; 
				LookVtc.Normalize(); 
				const FRotator LookRtr = LookVtc.ToOrientationRotator() ;
				FRotator DifLook = FRotator(0, LookRtr.Yaw - SkeletonComp->GetComponentRotation().Yaw + Init_Skeleton_Yaw, 0);
				SkeletonComp->AddWorldRotation(DifLook);
			}
		}
	}

	//Update HUD Value calling Blueprint functions
	if (HUD_Inst)
	{
		FOutputDeviceNull ar; 
		FString cmd = FString::Printf(TEXT("SetLife %f"), GetLifePercent()); 
		HUD_Inst->CallFunctionByNameWithArguments(*cmd, ar, NULL, true);

		float heatpercent = Current_HeatValue / Max_HeatValue;
		if(has_overheated)
		{
			FString cmd2 = FString::Printf(TEXT("SetHeat %f true"), heatpercent);		
			HUD_Inst->CallFunctionByNameWithArguments(*cmd2, ar, NULL, true);	
		}
		else
		{
			FString cmd2 = FString::Printf(TEXT("SetHeat %f false"), heatpercent);
			HUD_Inst->CallFunctionByNameWithArguments(*cmd2, ar, NULL, true);
		}

		FString cmd3 = FString::Printf(TEXT("SetScore %i"), Score);
		HUD_Inst->CallFunctionByNameWithArguments(*cmd3, ar, NULL, true);

	}
}

// Called to bind functionality to input
void ATDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	PlayerInputComponent->BindAxis("Forward", this, &ATDCharacter::MoveForward); 
	PlayerInputComponent->BindAxis("Right", this, &ATDCharacter::MoveRight);
	PlayerInputComponent->BindAxis("FireAxis", this, &ATDCharacter::FireInput);
}

void ATDCharacter::GetDamage(float value)
{
	Current_Life -= value; 

	UGameplayStatics::PlaySound2D(GetWorld(), Hurt_Sound);

	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (camManager)
	{
		camManager->PlayCameraShake(DamagedCameraShake);
	}

}

float ATDCharacter::GetLifePercent()
{
	return Current_Life/Max_Life;
}

