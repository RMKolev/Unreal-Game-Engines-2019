// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Abilities/Ability.h"
#include "TopDownARPGCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Triggers/TimePoint.h"
#include "Triggers/TimePointData.h"
#include "TimeTear.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNARPG_API UTimeTear : public UAbility
{
	GENERATED_BODY()
public:
	virtual void BeginPlay(AActor* Source) override;
	virtual void Activate(AActor* Source) override;
protected:
	virtual void ClearResidualEffects();
	virtual void OnSpawnTimePoint();
	virtual void SetActorTimeTearEffects();
private:
	FVector CurrDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float TimeDilation = 0.0001f;

	FTimerHandle TimePointSpawnTimerHandle;
	FTimerHandle ActiveSkillTimerHandle;
	FTimerHandle ActorTimeDilationIncrease;

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<ATimePoint> TimeCapsuleBP;


	ATopDownARPGCharacter* Owner = nullptr;

	UPROPERTY(EditAnywhere)//Turns back time X seconds back
	float SecondsToRewind = 10.f; 

	UPROPERTY(EditAnywhere)// Attempts to push the ability into this time. Due to physical limitations it will be considered more of a guideline than a hard truth.
	float SecondsToRunAbility = 10.f; 

	const float TimePointInterval = 0.05f; //For stability's sake this should remain unchanged unless thoroughly tested
	TCircularBuffer<FTimePointData> points = TCircularBuffer<FTimePointData>( (uint32)((SecondsToRewind*100)/(TimePointInterval*100)));
	uint32 currentIndex = 0;
	uint32 hopCount = SecondsToRewind/TimePointInterval;
	uint32 currentHop = 0;
	uint32 bEndAbility : 1;

	FVector getNextInterpolationPoint();
	float alpha = 0.f;
	void InterpolateCast();
};
