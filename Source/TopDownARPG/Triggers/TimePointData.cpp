// Fill out your copyright notice in the Description page of Project Settings.


#include "TimePointData.h"

FTimePointData::FTimePointData()
{
}

FTimePointData::FTimePointData(const FVector & location, const float & health) : Location(location), Health(health)
{
}

FTimePointData::~FTimePointData()
{
}

FVector FTimePointData::GetLocation() const
{
	return Location;
}

float FTimePointData::GetHealth() const
{
	return Health;
}
