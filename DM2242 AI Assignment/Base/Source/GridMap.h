#ifndef GRIDMAP_H
#define GRIDMAP_H

class GridMap
{
    // number of rows and columns of grids
    const int m_rows;
    const int m_columns;

public:
    bool **m_collisionGrid;

    GridMap(const int rows = 20, const int cols = 20);
    ~GridMap();

    void Update();  // Update collision grid

    int GetRows() const;
    int GetColumns() const;
};

#endif