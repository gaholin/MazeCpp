// Maze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Options.h"
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Cell.h"
#include <list>
using std::list;


class Maze : public olc::PixelGameEngine
{
public:
    Cell *grid[NUM_COLS*NUM_ROWS];
    Cell *first, *last;
    Cell *current;
    list<Cell*> m_Stack;
    int State;

    enum STATE
    {
        STATE_GEN = 0,
        STATE_POSTPROCESS,
        STATE_TRACEBACK,
        STATE_DONE,
    };
    
    Maze()
    {
        sAppName = "Maze";
        State = STATE_GEN;
    }

public:
    bool OnUserCreate() override
    {
        // create the grid
        for (int j = 0; j < NUM_ROWS; j++)
        {
            for (int i = 0; i < NUM_COLS; i++)
            {
                grid[i + j * NUM_COLS] = new Cell(i, j);
            }
        }
        // set begginer
        current = grid[0];
        current->visiabled = true;
        current->start = true;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        for (int j = 0; j < NUM_ROWS; j++)
        {
            for (int i = 0; i < NUM_COLS; i++)
            {
                Cell block = *grid[i + j * NUM_COLS];
                if (block.start)
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::BLUE);
                else if (block.goal)
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::RED);
                else if (block.path)
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::VERY_DARK_BLUE);
                else if (block.highlight)
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::Pixel(255, 128, 128));
                else if (block.visiabled)
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::VERY_DARK_GREEN);
                else
                    FillRect(block.i*CELL_PIXEL, block.j*CELL_PIXEL, CELL_PIXEL, CELL_PIXEL, olc::BLACK);
                if (block.walls[0])
                    DrawLine(block.i*CELL_PIXEL, block.j*CELL_PIXEL, block.i*CELL_PIXEL + CELL_PIXEL - 1, block.j*CELL_PIXEL, olc::WHITE);
                if (block.walls[1])
                    DrawLine(block.i*CELL_PIXEL + CELL_PIXEL - 1, block.j*CELL_PIXEL, block.i*CELL_PIXEL + CELL_PIXEL - 1, block.j*CELL_PIXEL + CELL_PIXEL - 1, olc::WHITE);
                if (block.walls[2])
                    DrawLine(block.i*CELL_PIXEL + CELL_PIXEL - 1, block.j*CELL_PIXEL + CELL_PIXEL - 1, block.i*CELL_PIXEL, block.j*CELL_PIXEL + CELL_PIXEL - 1, olc::WHITE);
                if (block.walls[3])
                    DrawLine(block.i*CELL_PIXEL, block.j*CELL_PIXEL + CELL_PIXEL - 1, block.i*CELL_PIXEL, block.j*CELL_PIXEL, olc::WHITE);
            }
        }
        if (State == STATE_GEN)
        {
            int index = current->Neighbors(grid);
            if (index != -1)
            {
                Cell *next = grid[index];
                current->highlight = false;
                current->RemoveWalls(next);
                next->setDistance(current->distance + 1);
                // push current to stack
                m_Stack.push_back(current);
                current = next;
                current->visiabled = true;
                current->highlight = true;

            }
            else if (m_Stack.size() > 0)
            {
                current->highlight = false;
                // pop from stack
                current = m_Stack.back();
                m_Stack.pop_back();
            }
            else
                State = STATE_POSTPROCESS;
        }
        else if (State == STATE_POSTPROCESS)
        {
            //set goal
            int distance = 0;
            int index = 0;
            for (int i = 0; i < NUM_COLS * NUM_ROWS; i++)
            {
                if (grid[i]->distance > distance)
                {
                    index = i;
                    distance = grid[i]->distance;
                }
            }
            grid[index]->goal = true;
            current = grid[index];
            State = STATE_TRACEBACK;
        }
        else if (State == STATE_TRACEBACK)
        {
            int index = current->findPath(grid);
            if (index != -1)
            {
                current = grid[index];
                current->path = true;
            }
            else
                State = STATE_DONE;
        }
        return true;
    }
};


int main()
{
    Maze demo;
    if (demo.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 2, 2))
        demo.Start();

    return 0;
}

