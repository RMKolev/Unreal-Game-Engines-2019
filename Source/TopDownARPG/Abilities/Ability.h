// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UAbility : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
    float CooldownTime = 1.f;
    UPROPERTY(EditAnywhere)
    float CooldownTimeDilation = 1.f;

    UFUNCTION(BlueprintCallable)
    virtual void Activate(AActor* Source);

    UFUNCTION(BlueprintImplementableEvent)
    void OnActivateBlueprint(AActor* Source);

    UFUNCTION(BlueprintCallable)
    bool IsOffCooldown() const { return bIsOffCooldown; }
	virtual void BeginPlay(AActor* Source) {}; // As the abilities have no representation in the engine world, it would be nice to know who they belong to. For use in events outside Activate.
    virtual void BeginDestroy();
protected:
	FTimerManager* TimerManager = nullptr; // Why? So the children functions can also use timed events
    bool bIsOffCooldown = true;
    
    FTimerHandle CooldownTimerHandle;
    void OnCooldownTimerExpired();
};
