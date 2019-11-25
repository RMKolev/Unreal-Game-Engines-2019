// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TOPDOWNARPG_API FTimePointData
{
public:
	FTimePointData();
	FTimePointData(const FVector& location, const float& health);
	~FTimePointData();

	FVector GetLocation() const;
	float GetHealth() const;
private:
	FVector location;
	float health = 0.f;
};
