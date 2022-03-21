#pragma once
#include "SteeringComponent.h"
class FleeComponent :
	public SteeringComponent
{
public:
	MathLibrary::Vector2 calculateForce() override;
};

