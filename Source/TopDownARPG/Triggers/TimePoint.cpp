// Fill out your copyright notice in the Description page of Project Settings.

#include "TimePoint.h"
#include "CoreMinimal.h"
#include "TopDownARPG.h"
#include "Components/SphereComponent.h"

// Sets default values
ATimePoint::ATimePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void ATimePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bActive) 
	{
		if (Ttl > 0)
		{
			Ttl -= DeltaTime;
		}
		else if (Ttl < 0) {
			Destroy();
		}
	}
}
void ATimePoint::BeginDestroy() {
	Super::BeginDestroy();
}
void ATimePoint::Activate()
{
	bActive = true;
	if (Next.IsValid()) 
	{
		Next->Activate();
	}
}

void ATimePoint::SetHealth(float health)
{
	this->HealthStat = health > 0 ? health : 1;
}

void ATimePoint::SetNext(TWeakObjectPtr<ATimePoint>& next)
{
	this->Next = next;
}

TWeakObjectPtr<ATimePoint> ATimePoint::GetNext()
{
	return this->Next;
}

float ATimePoint::GetHealth() const
{
	return this->HealthStat;
}

