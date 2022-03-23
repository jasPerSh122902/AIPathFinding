#include "Ghost.h"
#include "MazeScene.h"
#include "Wall.h"
#include "raylib.h"
#include "Transform2D.h"
#include "PathfindComponent.h"
#include "StateMachineComponent.h"
#include "FleeComponent.h"
#include "SeekComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Ghost::Ghost(float x, float y, float maxSpeed, float maxForce, int color, Maze* maze)
	: Agent(x, y, "Ghost", maxSpeed, maxForce)
{
	m_maze = maze;
	getTransform()->setScale({ Maze::TILE_SIZE,Maze::TILE_SIZE });

	
	m_pathfindComponent = new PathfindComponent(maze);
	m_pathfindComponent->setColor(color);
	addComponent(m_pathfindComponent);
	addComponent(new SpriteComponent("Images/enemy.png"));
}

Ghost::~Ghost()
{
	delete m_pathfindComponent;
}

void Ghost::start()
{
	Agent::start();
	
	m_seekComponent = addComponent<SeekComponent>();
	m_seekComponent->setSteeringForce(100);
	m_seekComponent->setTarget(m_target);
	m_fleeComponent = addComponent<FleeComponent>();
	m_fleeComponent->setSteeringForce(100);
	m_fleeComponent->setTarget(m_target);

	m_stateMachine = addComponent<StateMachineComponent>();
}

void Ghost::update(float deltaTime)
{
	Agent::update(deltaTime);
}

void Ghost::draw()
{
	Agent::draw();
}

void Ghost::onCollision(Actor* other)
{
	if (Wall* wall = dynamic_cast<Wall*>(other)) {
		MathLibrary::Vector2 halfTile = { Maze::TILE_SIZE / 2.0f, Maze::TILE_SIZE / 2.0f };
		MathLibrary::Vector2 position = getTransform()->getWorldPosition();
		position = position + halfTile;
		MathLibrary::Vector2 tilePosition = {
			roundf(position.x / Maze::TILE_SIZE) * Maze::TILE_SIZE,
			roundf(position.y / Maze::TILE_SIZE) * Maze::TILE_SIZE
		};
		tilePosition = tilePosition - halfTile;
		getTransform()->setWorldPostion(tilePosition);

		getMoveComponent()->setVelocity({ 0, 0 });
	}
}

void Ghost::setTarget(Actor* target)
{
	m_target = target;
	m_pathfindComponent->setTarget(target);
}

Actor* Ghost::getTarget()
{
	return m_target;
}