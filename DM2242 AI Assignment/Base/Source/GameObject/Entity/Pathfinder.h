#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Vector3.h"
#include "Direction.h"
#include <vector>

struct Node
{
    Node* parent;

    // position in GridMap
    int row;
    int col;

    // heuristics
    int cost;   // how much it cost to get here from start point
    int distFromStart;  // how far it is from the start point
    int distToGoal;     // how far it is from the destination

    Node() {}
    Node(int row, int col) : row(row), col(col), parent(NULL)
    {}
    ~Node() {}

    int GetFValue()
    {
        return cost + distToGoal;
    }

    Vector3 GetPosition()
    {
        return Vector3(col, row, 0.f);
    }
};

class Pathfinder
{
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    // find current tile player is at
    // open list
    // closed list

    
    Vector3 destination;
    DIRECTION m_entityDir;  // direction of the Entity

public:
    std::vector<Node> foundPath;     // top Node in stack is the next step to take; destination at bottom of stack

    Vector3 position;

    Pathfinder();
    ~Pathfinder();

    void Update(double dt);

    bool IsPathEmpty(); // check whether there is a path or not
    void EmptyPath();   // empty the path of Nodes

    bool hasReachedNode(const Vector3& pos);    // reached the next node to go to; time to recalculate path
    bool hasReachedDestination(const Vector3& pos); // check if node reached is the destination

    void ReceiveDirection(DIRECTION dir);   // get Entity's current direction

    void ReceiveCurrentPos(const Vector3& pos);     // get the Entity's current position
    void ReceiveDestination(const Vector3& pos);    // when movementNodes is empty or state changes, receive the Entity's new destination
    void FindPathAStar();   // main loop to find path with A*; call this when entity is to change path (e.g. due to movement nodes being empty or change in state)
    void FindPathGreedyBestFirst();     // main loop to find path with Greedy Best First Search
    void MoveStraightLine();    // calculate the velocity and direction of the Entity using a given waypoint

    void AddFrontier();     // add a frontier at start of each loop
    int GetCurrentCost(Node* current);   // get movement cost for start point to current position
    int GetMovementCost(Node* current, Node* neighbour);    // get movement cost to move to neighbour based on current
    // -- in this case, movement cost between all grids are same, but movement cost will be higher if there is a need to change direction
    int GetDistToGoal(Node* node);      // get cost to get to destination from Node
};

#endif