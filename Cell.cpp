#include "Options.h"
#include "Cell.h"
#include <iostream>

Cell::Cell(int x, int y)
{
    i = x;
    j = y;
    visiabled = false;
    highlight = false;
    walls[0] = true;
    walls[1] = true;
    walls[2] = true;
    walls[3] = true;
    start = false;
    goal = false;
    path = false;
    distance = 0;
}

Cell::~Cell()
{
}

static int getIndex(int x, int y)
{
    if (x < 0 || y < 0 || x >= NUM_COLS || y >= NUM_ROWS)
        return -1;
    return x + y * NUM_COLS;
}

int Cell::Neighbors(Cell **grids)
{
    int index;
    int neighbors[4];
    int size = 0;
    // top
    index = getIndex(i, j - 1);
    if (index != -1 && !grids[index]->visiabled) {
        neighbors[size++] = index;
    }
    // right
    index = getIndex(i + 1, j);
    if (index != -1 && !grids[index]->visiabled) {
        neighbors[size++] = index;
    }
    // bottom
    index = getIndex(i, j + 1);
    if (index != -1 && !grids[index]->visiabled) {
        neighbors[size++] = index;

    }
    // left
    index = getIndex(i - 1, j);
    if (index != -1 && !grids[index]->visiabled) {
        neighbors[size++] = index;
    }
    if (size == 0)
        return -1;
    index = rand() % size;
    return neighbors[index];
}

void Cell::RemoveWalls(Cell *neighbor)
{
    int x = i - neighbor->i;
    if (x == 1)
    {
        walls[3] = false;
        neighbor->walls[1] = false;
    }
    else if (x == -1)
    {
        walls[1] = false;
        neighbor->walls[3] = false;
    }
    int y = j - neighbor->j;
    if (y == 1)
    {
        walls[0] = false;
        neighbor->walls[2] = false;
    }
    else if (y == -1)
    {
        walls[2] = false;
        neighbor->walls[0] = false;
    }
}

void Cell::setDistance(int dist)
{
    distance = dist;
}

int Cell::findPath(Cell **grids)
{
    int index;
    int neighbors[4];
    int size = 0;
    // top
    index = getIndex(i, j - 1);
    if (index != -1 && grids[index]->distance == distance - 1)
        return index;
    // right
    index = getIndex(i + 1, j);
    if (index != -1 && grids[index]->distance == distance - 1)
        return index;
    // bottom
    index = getIndex(i, j + 1);
    if (index != -1 && grids[index]->distance == distance - 1)
        return index;
    // left
    index = getIndex(i - 1, j);
    if (index != -1 && grids[index]->distance == distance - 1)
        return index;
    return -1;
}
