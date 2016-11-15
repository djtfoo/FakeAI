#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Vector3.h"
#include <vector>

struct Node
{
    Node* parent;

    //const Vector3 position;
    
    // position in GridMap
    int row;
    int col;

    // heuristics
    int cost;   // how much it cost to get here from start point
    int distToGoal;     // how far it is from the destination

    Node() {}
    Node(int row, int col) : row(row), col(col), parent(NULL)
    {}
    ~Node() {}

    int GetFValue()
    {
        return cost + distToGoal;
    }
};

class Pathfinder
{
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    // find current tile player is at
    // open list
    // closed list

    Vector3 position;
    Vector3 destination;

public:
    std::vector<Node> foundPath;     // top Node in stack is the next step to take; destination at bottom of stack

    Pathfinder();
    ~Pathfinder();

    void Update(double dt);

    bool hasFinishedMovement();
    void ReceiveCurrentPos(const Vector3& pos);
    void ReceiveDestination(const Vector3& pos);    // when movementNodes is empty or state changes, receive new destination
    void FindPathAStar();   // main loop to find path with A*; call this when entity is to change path (e.g. due to movement nodes being empty or change in state)
    void FindPathGreedyBestFirst();     // main loop to find path with Greedy Best First Search

    void AddFrontier();     // add a frontier at start of each loop
    int GetCurrentCost(Node* current);   // get movement cost for start point to current position
    int GetMovementCost(Node* current, Node* neighbour);    // get movement cost to move to neighbour based on current
    // -- in this case, movement cost between all grids are same, but movement cost will be higher if there is a need to change direction
    int GetDistToGoal(Node* node);      // get cost to get to destination from Node
};

#endif