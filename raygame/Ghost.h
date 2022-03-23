#pragma once
#include "Agent.h"

class Maze;
class PathfindComponent;
class StateMachineComponent;
class FleeComponent;
class SeekComponent;

class Ghost : public Agent
{
public:
	/// <param name="x">The x-position</param>
	/// <param name="y">The y-position</param>
	/// <param name="speed">The maximum movement speed (magnitude)</param>
	/// <param name="color">The color in 0xRRGGBBAA</param>
	/// <param name="maze">The maze used for pathfinding</param>
	Ghost(float x, float y, float speed, float maxForce, int color, Maze* maze);
	~Ghost();

	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual void onCollision(Actor* other) override;

	/// <returns>The current target</returns>
	Actor* getTarget();
	/// <summary>
	/// Set the target of the ghost
	/// </summary>
	/// <param name="target">The new target</param>
	void setTarget(Actor* target);

private:
	StateMachineComponent* m_stateMachine;
	PathfindComponent* m_pathfindComponent;
	FleeComponent* m_fleeComponent;
	SeekComponent* m_seekComponent;
	Maze* m_maze;

	Actor* m_target = nullptr;
};

