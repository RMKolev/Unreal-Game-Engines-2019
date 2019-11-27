// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TimePoint.generated.h"

UCLASS()
class TOPDOWNARPG_API ATimePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimePoint();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Activate();
	void SetHealth(float health);
	void SetNext(TWeakObjectPtr<ATimePoint>& next);
	TWeakObjectPtr<ATimePoint> GetNext();
	float GetHealth() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
private:
	float HealthStat = 0.0f; // This should be changed to PlayerStat or some kind of Stats the timeCapsule should keep when more stats arise
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, Category = Gameplay)
	float Ttl = 2.0f;
	bool bActive = false;
	TWeakObjectPtr<ATimePoint> Next;

};
