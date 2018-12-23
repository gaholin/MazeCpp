#pragma once
class Cell
{
public:
    Cell(int x, int y);
    ~Cell();
    int Neighbors(Cell** grids);
    void RemoveWalls(Cell *neighbor);
    void setDistance(int dist);
    int findPath(Cell **grids);
public:
    int i;
    int j;
    bool visiabled;
    bool highlight;
    bool walls[4];
    bool start;
    bool goal;
    bool path;
    int distance;
};

