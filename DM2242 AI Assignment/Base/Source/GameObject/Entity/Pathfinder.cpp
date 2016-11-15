#include "Pathfinder.h"
#include "../../SharedData.h"

Pathfinder::Pathfinder()
{
}

Pathfinder::~Pathfinder()
{
}

void Pathfinder::Update(double dt)
{

}

bool Pathfinder::hasFinishedMovement()
{
    return true;
}

void Pathfinder::ReceiveCurrentPos(const Vector3& pos)
{
    position = pos;
}

void Pathfinder::ReceiveDestination(const Vector3& pos)
{
    destination = pos;
}

void Pathfinder::FindPathAStar()
{
    Node* startNode = new Node((int)position.y, (int)position.x);
    startNode->cost = 0;
    startNode->distToGoal = GetDistToGoal(startNode);
    openList.push_back(startNode);

    // "best" Node (Node with lowest F Value)
    int lowestFValue = openList[0]->GetFValue(), lowestFValueIndex = 0;

    int destinationRow = (int)(position.y);
    int destinationCol = (int)(position.x);

    bool b_pathNotFound = true;
    while (b_pathNotFound)
    {
        // Add frontier to open list
        AddFrontier();

        // find for lowest F value from open list
        // - first time this runs, only starting node is in open list

        Node* bestNode = openList[lowestFValueIndex];

        // check if it is the goal - if it is, get the path and break
        if (bestNode->row == destinationRow && bestNode->col == destinationCol)
        {
            // path found
            b_pathNotFound = false;
            break;
        }
        
        // else, remove from open list
        closedList.push_back(bestNode);

        // check its neighbours for way to proceed with best path
        // Check neighbour1

        // Check neighbour2

        // Check neighbour3

        // Check neighbour4
    }

    // reconstruct path
    Node* finalNode = openList[lowestFValueIndex];
}

void Pathfinder::FindPathGreedyBestFirst()
{
    Node* startNode = new Node((int)position.y, (int)position.x);
    startNode->cost = 0;
    startNode->distToGoal = GetDistToGoal(startNode);
    openList.push_back(startNode);

    Node* currentNode = startNode;  // replaces openList

    int destinationRow = (int)(destination.y);
    int destinationCol = (int)(destination.x);

    bool b_pathNotFound = true;
    if (startNode->row == destinationRow && startNode->col == destinationCol)
    {
        // check if starting node is the destination
        b_pathNotFound = false;
    }

    while (b_pathNotFound)
    {
        // check if neighbour is a collidable grid

        // check its neighbours for way to proceed with best path
        std::vector<Node*> neighbours;
        Node* neighbour;

        if (currentNode->col + 1 < SharedData::GetInstance()->m_gridMap->GetColumns())
        {
            neighbour = new Node(currentNode->row, currentNode->col + 1);
            neighbour->cost = currentNode->cost + GetMovementCost(currentNode, neighbour);
            neighbour->distToGoal = GetDistToGoal(neighbour);
            neighbours.push_back(neighbour);
        }
        
        if (currentNode->row + 1 < SharedData::GetInstance()->m_gridMap->GetRows())
        {
            neighbour = new Node(currentNode->row + 1, currentNode->col);
            neighbour->cost = currentNode->cost + GetMovementCost(currentNode, neighbour);
            neighbour->distToGoal = GetDistToGoal(neighbour);
            neighbours.push_back(neighbour);
        }

        if (currentNode->col - 1 >= 0)
        {
            neighbour = new Node(currentNode->row, currentNode->col - 1);
            neighbour->cost = currentNode->cost + GetMovementCost(currentNode, neighbour);
            neighbour->distToGoal = GetDistToGoal(neighbour);
            neighbours.push_back(neighbour);
        }

        if (currentNode->row - 1 >= 0)
        {
            neighbour = new Node(currentNode->row - 1, currentNode->col);
            neighbour->cost = currentNode->cost + GetMovementCost(currentNode, neighbour);
            neighbour->distToGoal = GetDistToGoal(neighbour);
            neighbours.push_back(neighbour);
        }

        // "best" Node (Node with lowest F Value)
        int lowestFValue = 999, lowestFValueIndex = 0;

        for (unsigned i = 0; i < neighbours.size(); ++i)
        {
            // check if it is the goal - if it is, get the path and break
            if (neighbours[i]->row == destinationRow && neighbours[i]->col == destinationCol)
            {
                // path found
                lowestFValueIndex = i;
                b_pathNotFound = false;
                break;
            }

            // check for collision
            if (!SharedData::GetInstance()->m_gridMap->m_collisionGrid[neighbours[i]->row][neighbours[i]->col])
            {
                if (neighbours[i]->GetFValue() < lowestFValue)
                {
                    lowestFValueIndex = i;
                    lowestFValue = neighbours[i]->GetFValue();
                }
            }
        }

        neighbours[lowestFValueIndex]->parent = currentNode;
        currentNode = neighbours[lowestFValueIndex];

        // push the neighbours into closed list;
        for (int i = neighbours.size() - 1; i >= 0; --i)
        {
            if (i != lowestFValueIndex)
            {
                // is not the current Node; put into closed list
                closedList.push_back(neighbours[i]);
            }
            neighbours.pop_back();
        }
    }

    // reconstruct path
    // puh Nodes along path
    for (Node* curr = currentNode; curr != NULL; curr = curr->parent)
    {
        foundPath.push_back(Node(curr->row, curr->col));
    }

    delete currentNode;

    // clear closedList Nodes
    while (!closedList.empty())
    {
        delete closedList.back();
        closedList.pop_back();
    }
}

void Pathfinder::AddFrontier()
{
    for (std::vector<Node*>::iterator it = closedList.begin(); it != closedList.end(); ++it)
    {
        // whether the neighbours can be added to the closed list because they're not inside yet
        bool neighbour1 = true, neighbour2 = true, neighbour3 = true, neighbour4 = true;

        // check if a Node's 4 neighbours not in closed list already
        for (std::vector<Node*>::iterator it2 = closedList.begin(); it2 != closedList.end(); ++it)
        {
            if (neighbour1)
            {
                if ((*it2)->row == (*it)->row && (*it2)->col == (*it)->col + 1
                    && (*it)->col + 1 < SharedData::GetInstance()->m_gridMap->GetColumns())
                    neighbour1 = false;
            }

            if (neighbour2)
            {
                if ((*it2)->row == (*it)->row + 1 && (*it2)->col == (*it)->col
                    && (*it)->row + 1 < SharedData::GetInstance()->m_gridMap->GetRows())
                    neighbour2 = false;
            }

            if (neighbour3)
            {
                if ((*it2)->row == (*it)->row && (*it2)->col == (*it)->col - 1
                    && (*it)->col >= 0)
                    neighbour3 = false;
            }

            if (neighbour4)
            {
                if ((*it2)->row == (*it)->row - 1 && (*it2)->col == (*it)->col
                    && (*it)->row >= 0)
                    neighbour4 = false;
            }
        }

        // check neighbour 1 is not in the closed list, and add to frontier
        if (neighbour1)
        {
            if (SharedData::GetInstance()->m_gridMap->m_collisionGrid[(*it)->row][(*it)->col + 1] == false)
            {
                // no collision, can walk on this Node
                Node* newFrontierNode = new Node((*it)->row, (*it)->col + 1);
                newFrontierNode->cost = (*it)->cost + GetMovementCost(*it, newFrontierNode);
                newFrontierNode->distToGoal = GetDistToGoal(newFrontierNode);
                openList.push_back(newFrontierNode);
            }
        }

        // check neighbour 2 is not in the closed list, and add to frontier
        if (neighbour2)
        {
            if (SharedData::GetInstance()->m_gridMap->m_collisionGrid[(*it)->row + 1][(*it)->col] == false)
            {
                // no collision, can walk on this Node
                Node* newFrontierNode = new Node((*it)->row + 1, (*it)->col);
                newFrontierNode->cost = (*it)->cost + GetMovementCost(*it, newFrontierNode);
                newFrontierNode->distToGoal = GetDistToGoal(newFrontierNode);
                openList.push_back(newFrontierNode);
            }
        }

        // check neighbour 3 is not in the closed list, and add to frontier
        if (neighbour3)
        {
            if (SharedData::GetInstance()->m_gridMap->m_collisionGrid[(*it)->row][(*it)->col - 1] == false)
            {
                // no collision, can walk on this Node
                Node* newFrontierNode = new Node((*it)->row, (*it)->col - 1);
                newFrontierNode->cost = (*it)->cost + GetMovementCost(*it, newFrontierNode);
                newFrontierNode->distToGoal = GetDistToGoal(newFrontierNode);
                openList.push_back(newFrontierNode);
            }
        }

        // check neighbour 4 is not in the closed list, and add to frontier
        if (neighbour4)
        {
            if (SharedData::GetInstance()->m_gridMap->m_collisionGrid[(*it)->row - 1][(*it)->col] == false)
            {
                // no collision, can walk on this Node
                Node* newFrontierNode = new Node((*it)->row - 1, (*it)->col);
                newFrontierNode->cost = (*it)->cost + GetMovementCost(*it, newFrontierNode);
                newFrontierNode->distToGoal = GetDistToGoal(newFrontierNode);
                openList.push_back(newFrontierNode);
            }
        }
    }
}

// NOT IN USE
int Pathfinder::GetCurrentCost(Node* current)
{
    // NOT IN USE
    return 1;
}

int Pathfinder::GetMovementCost(Node* current, Node* neighbour)
{
    return 1;
}

int Pathfinder::GetDistToGoal(Node* node)
{
    return abs(node->row - (int)(destination.y)) + abs(node->col - (int)(destination.x));
}