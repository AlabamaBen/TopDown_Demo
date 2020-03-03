// Fill out your copyright notice in the Description page of Project Settings.


#include "TDEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "TDEnemy_AIController.h"
#include "TimerManager.h"
#include "TDCharacter.h"
#include "Engine.h"





// Sets default values
ATDEnemy::ATDEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletonComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	SkeletonComp->SetupAttachment(RootComponent);

	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponComp->SetupAttachment(RootComponent);


	AIControllerClass = TSubclassOf<ATDEnemy_AIController>();
}

// Called when the game starts or when spawned
void ATDEnemy::BeginPlay()
{
	Super::BeginPlay();

	Init_Skeleton_Yaw = SkeletonComp->GetComponentRotation().Yaw;

	//Attach Grenade to skeleton hand
	if (WeaponComp)
	{
		const FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		WeaponComp->AttachToComponent(SkeletonComp, rules, "RGun");
	}
}

// Called every frame
void ATDEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATDEnemy::Die()
{
	if (!dying)
	{
		dying = true; 

		//Stop controller
		GetController()->StopMovement(); 

		//Stop render meshs
		SkeletonComp->SetVisibility(false); 
		WeaponComp->SetVisibility(false);
		GetCapsuleComponent()->SetActive(false);

		//Play explosion fx and sound
		FTransform SpawnTransform = GetActorTransform(); 
		SpawnTransform.SetScale3D(FVector(DiePS_Size, DiePS_Size, DiePS_Size));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DiePS, SpawnTransform); 
		UGameplayStatics::PlaySound2D(GetWorld(), Die_Sound);

		//Trace sphere around 
		TArray<FHitResult> HitOut;
		FHitResult EmptyHit;
		ECollisionChannel CollisionChannel = ECollisionChannel::ECC_WorldStatic;
		FCollisionQueryParams TraceParams;
		FCollisionResponseParams ExtraParams;
		bool hit = GetWorld()->SweepMultiByChannel(
			HitOut, GetActorLocation(), GetActorLocation(),
			FQuat::Identity, CollisionChannel,
			FCollisionShape::MakeSphere(300.0f),
			TraceParams, ExtraParams
		);

		//If TDCharacter is in it, make damages
		if (hit == true)
		{

			for (auto& ActorHit : HitOut)
			{
				ATDCharacter* HitCharacter = Cast<ATDCharacter>(ActorHit.GetActor()); 
				if (HitCharacter)
				{
					HitCharacter->GetDamage(20); 
				}
			}

		}

		//Self Destroy
		Destroy();
	}
	
}


