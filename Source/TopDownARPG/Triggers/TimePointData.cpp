// Fill out your copyright notice in the Description page of Project Settings.


#include "TimePointData.h"

FTimePointData::FTimePointData()
{
}

FTimePointData::FTimePointData(const FVector & location, const float & health) : location(location), health(health)
{
}

FTimePointData::~FTimePointData()
{
}

FVector FTimePointData::GetLocation() const
{
	return location;
}

float FTimePointData::GetHealth() const
{
	return health;
}
