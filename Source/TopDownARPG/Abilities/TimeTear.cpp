// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Engine.h"
#include "TimeTear.h"
#include "AIController.h"
#include "TopDownARPG.h"
#include "TopDownARPGPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTimeTear::BeginPlay(AActor* Source) {
	this->CooldownTime = 1.5 * SecondsToRewind; // Seems fair. We don't want to spam time rewind.
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UAbility::Activate IsValid(World) == false"));
		return;
	}
	this->Owner = Cast<ATopDownARPGCharacter>(Source);
	if (Owner == nullptr) {
		UE_LOG(LogTopDownARPG, Log, TEXT("Invalid Ability: Time Tear can only be used on Characters!"));
			return;
	}
	TimerManager = &World->GetTimerManager();
	hopCount = SecondsToRewind / TimePointInterval;
	points = TCircularBuffer<FTimePointData>((uint32)((SecondsToRewind * 100) / (TimePointInterval * 100)));
	bIsOffCooldown = false;
	TimerManager->SetTimer(CooldownTimerHandle, this, &UTimeTear::OnCooldownTimerExpired, SecondsToRewind, false, SecondsToRewind); // Setting the initial cooldown to generate enough points from gamestart
	TimerManager->SetTimer(TimePointSpawnTimerHandle, this, &UTimeTear::OnSpawnTimePoint, TimePointInterval, true, 0.0f);
	UE_LOG(LogTopDownARPG, Log, TEXT("HopCount is %d, Points capacity is %d, SecondsToRewind is %f, SecondsToRunAbility is %f"), hopCount, points.Capacity(),SecondsToRewind,SecondsToRunAbility);
	Super::BeginPlay(Source);
}

void UTimeTear::OnSpawnTimePoint() {
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UAbility::Activate IsValid(World) == false"));
		return;
	}
	auto location = Owner->GetActorLocation();
	float health = Owner->GetHealth();
	points[currentIndex] = FTimePointData(location, health);
	currentIndex = points.GetNextIndex(currentIndex);
	//Use this code to generate a trace and see the points generated from the ability. Useful in debugging or for generating a cool trace. Could be used to add some effects later.
	/*if (TimeCapsuleBP) 
	{	
		auto location = Owner->GetActorLocation();
		if (Owner!=nullptr && Cast<ATopDownARPGCharacter>(Owner)!=nullptr) {
			health = Cast<ATopDownARPGCharacter>(Owner)->GetHealth();
		}

		ATimePoint* newPoint = Cast<ATimePoint>(World->SpawnActor(TimeCapsuleBP, &location));
	}*/ 
}
void UTimeTear::Activate(AActor * Source)
{
	if (this->bIsOffCooldown) {
		TimerManager->ClearTimer(TimePointSpawnTimerHandle);
		UWorld* World = GetWorld();
		if (IsValid(World) == false)
		{
			UE_LOG(LogTopDownARPG, Error, TEXT("UAbility::Activate IsValid(World) == false"));
			return;
		}
		Cast<ATopDownARPGPlayerController>(Owner->GetController())->StopFollowingMouse();
		UGameplayStatics::SetGlobalTimeDilation(World, TimeDilation);
		TimerManager->SetTimer(ActorTimeDilationIncrease, this, &UTimeTear::SetActorTimeTearEffects, 0.2f, false, 0.0f);
	}
	else {
		UE_LOG(LogTopDownARPG, Error, TEXT("Time Tear Ability is on Cooldown!"));
	}
}

void UTimeTear::SetActorTimeTearEffects()
{
	bEndAbility = false;
	UE_LOG(LogTopDownARPG, Error, TEXT("Actor was dilated!!!!"));
	currentIndex = points.GetPreviousIndex(currentIndex);
	Owner->CustomTimeDilation = 0.f;
	Owner->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	Owner->GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	Owner->GetCharacterMovement()->bRequestedMoveUseAcceleration = false;
	APlayerController* pc = Cast<APlayerController>(Owner->GetController());
	pc->bShowMouseCursor = false;
	pc->bEnableClickEvents = false;
	pc->DisableInput(pc); // Why isn't this a static method???
	pc->StopMovement();
	TimerManager->SetTimer(ActiveSkillTimerHandle, this, &UTimeTear::InterpolateCast, TimePointInterval*TimeDilation, true, 0.01f*TimeDilation);
}

void UTimeTear::ClearResidualEffects()
{
	UGameplayStatics::SetGlobalTimeDilation(GWorld, 1.f);
	Owner->SetHealth(points[currentIndex].GetHealth());
	currentHop = 0;
	currentIndex = 0;
	alpha = 0;
	TimerManager->ClearTimer(ActiveSkillTimerHandle);
	Owner->CustomTimeDilation = 1.f;
	Owner->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
	Owner->GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	Owner->GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	APlayerController* pc = Cast<APlayerController>(Owner->GetController());
	pc->bShowMouseCursor = true;
	pc->bEnableClickEvents = true;
	pc->EnableInput(pc);
	pc->StopMovement();
	bIsOffCooldown = false;
	TimerManager->SetTimer(TimePointSpawnTimerHandle, this, &UTimeTear::OnSpawnTimePoint, TimePointInterval, true, 0.0f);
	TimerManager->SetTimer(CooldownTimerHandle, this, &UTimeTear::OnCooldownTimerExpired, CooldownTime*CooldownTimeDilation, false, CooldownTime*CooldownTimeDilation);
}

FVector UTimeTear::getNextInterpolationPoint()
{
	alpha += SecondsToRewind / SecondsToRunAbility;
	UE_LOG(LogTopDownARPG, Log, TEXT("Alpha %f"), alpha);
	if (alpha >= 1) {
		while (alpha >= 1) {
			UE_LOG(LogTopDownARPG, Log, TEXT("Alpha %f"), alpha);
			UE_LOG(LogTopDownARPG, Log, TEXT("Alpha %f"), alpha);
			alpha--;
			currentHop++;
			if (currentHop >= hopCount) {
				UE_LOG(LogTopDownARPG, Log, TEXT("Current hop exceeded %d > %d"), currentHop, hopCount);
				bEndAbility = true;
				return Owner->GetActorLocation();
			}
			else {
				currentIndex = points.GetPreviousIndex(currentIndex);
			}
		}
	}
	return (1 - alpha)*points[currentIndex].GetLocation() + alpha * points[points.GetPreviousIndex(currentIndex)].GetLocation();
}

void UTimeTear::InterpolateCast() {
	FVector nextInterpolationPoint = getNextInterpolationPoint();
	CurrDirection = Owner->GetActorLocation() - nextInterpolationPoint;
	UE_LOG(LogTopDownARPG,Log,TEXT("Actor rotation is %s"), *CurrDirection.Rotation().ToString())
		if (!bEndAbility) {
			Owner->SetActorRotation(CurrDirection.Rotation().Quaternion());
		}
	Owner->SetActorLocation(nextInterpolationPoint);
	if (bEndAbility) {
		UE_LOG(LogTopDownARPG, Log, TEXT("ABILITY Finished at index %s"), *points[currentIndex].GetLocation().ToString());
		CurrDirection = FVector::ZeroVector; // Set the movement vector to zero so as not to get a random direction when the ability finishes!
		ClearResidualEffects();
	}
}