#include "StateMachineComponent.h"
#include "Actor.h"
#include "SeekComponent.h"
#include "FleeComponent.h"
#include "Transform2D.h"

void StateMachineComponent::start()
{
	Component::start();

	m_seekComponent = getOwner()->getComponent<SeekComponent>();
	m_seekForce = m_seekComponent->getSteeringForce();

	m_fleeComponent = getOwner()->getComponent<FleeComponent>();
	m_fleeingForce = m_fleeComponent->getSteeringForce();

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
	case SEEK:
		m_seekComponent->setSteeringForce(m_seekComponent->getSteeringForce());
		m_fleeComponent->setSteeringForce(0);

		if (distanceFromTarget <= 250 && acos(MathLibrary::Vector2::dotProduct(coneFlee, getOwner()->getTransform()->getForward()) < 1))
			setCurrentState(FLEE);
		break;
	case FLEE:
		m_seekComponent->setSteeringForce(0);
		m_fleeComponent->setSteeringForce(m_fleeComponent->getSteeringForce());

		if (!distanceFromTarget <= 250 && acos(MathLibrary::Vector2::dotProduct(coneFlee, getOwner()->getTransform()->getForward()) < 1))
			setCurrentState(SEEK);
		break;
	}
}
