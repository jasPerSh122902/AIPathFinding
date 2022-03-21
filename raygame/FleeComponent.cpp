#include "FleeComponent.h"
#include "Actor.h"
#include "Transform2D.h"
#include "Agent.h"
#include "MoveComponent.h"
MathLibrary::Vector2 FleeComponent::calculateForce()
{
	if (!getTarget())
		return { 0,0 };

	MathLibrary::Vector2 directionToTarget = getOwner()->getTransform()->getWorldPosition()-
		getTarget()->getTransform()->getWorldPosition();

	MathLibrary::Vector2 desriredVelocity = directionToTarget.getNormalized() * getSteeringForce();
	MathLibrary::Vector2 seekForce = desriredVelocity - getAgent()->getMoveComponent()->getVelocity();

	return seekForce;
}
