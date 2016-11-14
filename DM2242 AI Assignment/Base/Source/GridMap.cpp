#include "GridMap.h"
#include "GameObject/GameObject.h"

GridMap::GridMap(const int rows, const int cols) : m_rows(rows), m_columns(cols)
{
    m_collisionGrid = new bool*[rows];
    for (int i = 0; i < rows; ++i)
    {
        m_collisionGrid[i] = new bool[cols];
        for (int j = 0; j < cols; ++j)
        {
            m_collisionGrid[i][j] = false;
        }
    }
}

GridMap::~GridMap()
{
    for (int i = 0; i < m_rows; ++i)
    {
        delete[] m_collisionGrid[i];
    }

    delete[] m_collisionGrid;
}

void GridMap::Update()
{
    // update the collision grid
}

int GridMap::GetRows() const
{
    return m_rows;
}

int GridMap::GetColumns() const
{
    return m_columns;
}