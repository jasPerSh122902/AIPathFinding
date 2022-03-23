#pragma once
#include "Component.h"

enum State
{
	SEEK,
	FLEE
};

class SeekComponent;
class FleeComponent;

class StateMachineComponent :
	public Component
{
public:
	void start() override;

	State getCurrentState() { return m_currentState; }
	void setCurrentState(State state) { m_currentState = state; }

	void update(float deltaTime) override;

private:
	State m_currentState;
	SeekComponent* m_seekComponent;
	FleeComponent* m_fleeComponent;
	float m_seekForce;
	float m_fleeingForce;
	float m_seekRange = 150;
};

