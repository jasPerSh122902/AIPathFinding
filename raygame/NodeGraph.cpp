#include "NodeGraph.h"
#include <raylib.h>
#include <xlocinfo>

DynamicArray<NodeGraph::Node*> reconstructPath(NodeGraph::Node* start, NodeGraph::Node* end)
{
	DynamicArray<NodeGraph::Node*> path;
	NodeGraph::Node* currentNode = end;

	while (currentNode != start->previous)
	{
		currentNode->color = 0x00FF00FF;
		start->color = 0xF00FFFFF;
		path.insert(currentNode, 0);
		currentNode = currentNode->previous;
	}

	return path;
}

float diagonalDistance(NodeGraph::Node* node, NodeGraph::Node* goal, float cardinalCost, float diagonalCost)
{
	float displacementX = abs(node->position.x - goal->position.x);
	float displacementY = abs(node->position.y - goal->position.y);

	return cardinalCost * (displacementX + displacementY) + (diagonalCost - 2 * cardinalCost) * fmin(displacementX, displacementY);
}
/// <summary>
/// sorts the nodes by the F score
/// </summary>
/// <param name="nodes">is the name for a list</param>
void sortFScore(DynamicArray<NodeGraph::Node*>& nodes)
{
	NodeGraph::Node* key = nullptr;
	int j = 0;

	for (int i = 1; i < nodes.getLength(); i++) {
		key = nodes[i];
		j = i - 1;
		while (j >= 0 && nodes[j]->fScore > key->fScore) {
			nodes[j + 1] = nodes[j];
			j--;
		}

		nodes[j + 1] = key;
	}
}
/// <summary>
/// sorts the nodes by the g score
/// </summary>
/// <param name="nodes">is the name for a list</param>
void sortGScore(DynamicArray<NodeGraph::Node*>& nodes)
{
	NodeGraph::Node* key = nullptr;
	int j = 0;

	for (int i = 1; i < nodes.getLength(); i++) {
		key = nodes[i];
		j = i - 1;
		while (j >= 0 && nodes[j]->gScore > key->gScore) {
			nodes[j + 1] = nodes[j];
			j--;
		}

		nodes[j + 1] = key;
	}
}

/// <summary>
/// finds the closes path to the player (this looks mostly like a state line).
/// </summary>
/// <param name="start">The begining node and the start of the path</param>
/// <param name="goal">the end of the path</param>
/// <returns> reconstructPath(start, goal) this function is to make the path</returns>
DynamicArray<NodeGraph::Node*> NodeGraph::findPath(Node* start, Node* goal)
{
	//makes all of the graph scores go back to zero
	resetGraphScore(start);
	float gScore = 0;
	NodeGraph::Node* m_currentNode;
	///initialization two dynamic Arrays for holding
	/// <param name="openList">This array or list is the first thing that is checked. Holds nodes that have not been touched</param>
	/// <param name="closedList">Holds the nodes that have been touched so to preven overlaping</param>
	DynamicArray<NodeGraph::Node*> openList, closedList = DynamicArray<NodeGraph::Node*>();
	openList.addItem(start);
	m_currentNode = start;
	//has to check to see if the openlist is empty
	while (openList.getLength() > 0)
	{
		//sorts the openlist by there f score
		sortFScore(openList);
		//makes the currentnode to the openlist at the index 0
		m_currentNode = openList[0];

		//returns if the currentnode is the goal
		if (m_currentNode == goal)
			return reconstructPath(start, m_currentNode);

		closedList.addItem(m_currentNode);
		openList.remove(m_currentNode);
		gScore = m_currentNode->gScore;

		//gos through the openlists edges 
		for (int n = 0; n < m_currentNode->edges.getLength(); n++) 
		{
			NodeGraph::Node* targetNode = m_currentNode->edges[0].target;
			//if the currentnode is not walkable
			if (m_currentNode->walkable == false)
				continue;//do not move
			//tries to see if the two list contains the target node
			if (!closedList.contains(m_currentNode->edges[n].target))
			{
				targetNode->gScore = m_currentNode->edges[n].cost + m_currentNode->gScore;
				targetNode->hScore = manHattan_Distance(m_currentNode->edges[n].target, goal);
				//makes the gscore of the targetnode equal to the openlist index 0 gscorce + the edges index N of the openlist index 0 at  cost
			}
			else 
				continue;
			//Adds the node to the open list if it is not already in it
			if (!openList.contains(targetNode->edges[n].target) || m_currentNode->edges[n].target->gScore > gScore)
			{
				//adds the target node to the openlist
				openList.addItem(m_currentNode->edges[n].target);
				m_currentNode->edges[n].target->color = 0x0FFFFF;//changes color
				//gets the currentNode edges index Ns target score and set it to the targetNode score
				m_currentNode->edges[n].target->gScore = targetNode->gScore;
				m_currentNode->edges[n].target->hScore = targetNode->hScore;
				//adds the g and h score of target node to the currentNnodes f score
				m_currentNode->edges[n].target->fScore = targetNode->gScore + targetNode->hScore;
				// gets the currentnodes edges targets previous and sets it to the currentNode
				m_currentNode->edges[n].target->previous = m_currentNode; 
			}
		}
	}
	return reconstructPath(start, goal);
}
/// <summary>
/// get the distance for the manHattan by taking in a left and right node
/// </summary>
/// <param name="left">The node on the left </param>
/// <param name="right">the node on the right</param>
/// <returns> gives a float from the two nodes</returns>
float NodeGraph::manHattan_Distance(Node* left, Node* right)
{
	return abs(right->position.x - left->position.x) + abs(right->position.y - left->position.y);
}

void NodeGraph::drawGraph(Node* start)
{
	DynamicArray<Node*> drawnList = DynamicArray<Node*>();
	drawConnectedNodes(start, drawnList);
}

void NodeGraph::drawNode(Node* node, float size)
{
	static char buffer[10];
	sprintf_s(buffer, "%.0f", node->gScore);

	//Draw the circle for the outline
	DrawCircle((int)node->position.x, (int)node->position.y, size + 1, GetColor(node->color));
	//Draw the inner circle
	DrawCircle((int)node->position.x, (int)node->position.y, size, GetColor(node->color));
	//Draw the text
	DrawText(buffer, (int)node->position.x, (int)node->position.y, .8f, BLACK);
}

void NodeGraph::drawConnectedNodes(Node* node, DynamicArray<Node*>& drawnList)
{
	drawnList.addItem(node);
	if (node->walkable)
		drawNode(node, 8);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		Edge e = node->edges[i];
		////Draw the Edge
		//DrawLine((int)node->position.x, (int)node->position.y, (int)e.target->position.x, (int)e.target->position.y, WHITE);
		////Draw the cost
		//MathLibrary::Vector2 costPos = { (node->position.x + e.target->position.x) / 2, (node->position.y + e.target->position.y) / 2 };
		//static char buffer[10];
		//sprintf_s(buffer, "%.0f", e.cost);
		//DrawText(buffer, (int)costPos.x, (int)costPos.y, 16, RAYWHITE);
		//Draw the target node
		if (!drawnList.contains(e.target)) {
			drawConnectedNodes(e.target, drawnList);
		}
	}
}

void NodeGraph::resetGraphScore(Node * start)
{
	DynamicArray<Node*> resetList = DynamicArray<Node*>();
	resetConnectedNodes(start, resetList);
}

void NodeGraph::resetConnectedNodes(Node* node, DynamicArray<Node*>& resetList)
{
	resetList.addItem(node);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		//changes the colors of the edges
		node->edges[i].target->gScore = 0x00FF00FF;
		node->edges[i].target->hScore = 0xFF0000FF;
		node->edges[i].target->fScore = 0x00FF00FF;
		node->edges[i].target->color = 0xFFFFFFFF;

		//Draw the target node
		if (!resetList.contains(node->edges[i].target)) {
			resetConnectedNodes(node->edges[i].target, resetList);
		}
	}
}