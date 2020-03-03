// Fill out your copyright notice in the Description page of Project Settings.


#include "TDAnimationInstance.h"
#include "TDCharacter.h"
#include "GameFramework/Pawn.h"

UTDAnimationInstance::UTDAnimationInstance()
{
	Speed = 0.0; 
	Direction = 0.0;

}

void UTDAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds); 
	APawn* OwningActor = TryGetPawnOwner();
	ATDCharacter* Character = Cast<ATDCharacter>(TryGetPawnOwner());


	
	if(OwningActor != nullptr && Character != nullptr)
	{
		FVector VelocityVect = OwningActor->GetVelocity();

		//Set speed to owner velocity
		Speed = VelocityVect.Size();

		VelocityVect.Normalize();

		//Compute angle between direction and velocity
		float fDotProduct = FVector::DotProduct(VelocityVect, Character->SkeletonComp->GetComponentRotation().Vector());

		//Compute sign of the angle
		FVector fCrossProduct = FVector::CrossProduct(VelocityVect, Character->SkeletonComp->GetComponentRotation().Vector());
		float dir = FVector::DotProduct(fCrossProduct, FVector::UpVector);

		if (dir < 0)
		{
			Direction = FMath::RadiansToDegrees(FMath::Acos(fDotProduct) );
		}
		else
		{
			Direction = FMath::RadiansToDegrees(FMath::Acos(fDotProduct)) * -1 ;
		}
		
		Direction += Character->Init_Skeleton_Yaw;

		//Clamp the value to provide from -180
		if (Direction < -160)
		{
			Direction = 180;
		}
		if (Direction > 160)
		{
			Direction = 180;
		}

		IsDead = Character->IsDead; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Owning Actor for AnimInstance")); 
	}


}
