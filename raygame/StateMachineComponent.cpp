#include "StateMachineComponent.h"
#include "Actor.h"
#include "SeekComponent.h"
#include "WanderComponent.h"
#include "FleeComponent.h"
#include "Transform2D.h"

void StateMachineComponent::start()
{
	Component::start();

	m_seekComponent = getOwner()->getComponent<SeekComponent>();
	m_seekForce = m_seekComponent->getSteeringForce();

	m_wanderComponent = getOwner()->getComponent<WanderComponent>();
	m_wanderForce = m_wanderComponent->getSteeringForce();

	m_currentState = SEEK;
}

void StateMachineComponent::update(float deltaTime)
{
	Component::update(deltaTime);

	MathLibrary::Vector2 targetPos = m_seekComponent->getTarget()->getTransform()->getWorldPosition();
	MathLibrary::Vector2 ownerPos = getOwner()->getTransform()->getWorldPosition();
	float distanceFromTarget = (targetPos - ownerPos).getMagnitude();

	MathLibrary::Vector2 coneFlee = (targetPos - ownerPos).getNormalized();
	bool targetInRange = distanceFromTarget <= m_seekRange;

	switch (m_currentState)
	{
	case WANDER:
		m_seekComponent->setSteeringForce(0);
		m_wanderComponent->setSteeringForce(m_wanderForce);
		m_fleeComponent->setSteeringForce(0);

		if (targetInRange)
			setCurrentState(SEEK);

		break;
	case SEEK:
		m_seekComponent->setSteeringForce(m_seekForce);
		m_wanderComponent->setSteeringForce(0);
		m_fleeComponent->setSteeringForce(0);

		if (distanceFromTarget <= 250 && acos(MathLibrary::Vector2::dotProduct(coneFlee, getOwner()->getTransform()->getForward()) < 1))
			setCurrentState(FLEE);

	case FLEE:
		m_seekComponent->setSteeringForce(0);
		m_wanderComponent->setSteeringForce(0);
		m_fleeComponent->setSteeringForce(m_fleeingForce);
		break;
	}
}
