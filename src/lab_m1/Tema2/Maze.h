#pragma once

#include <vector>
#include "lab_m1/Tema2/defines.h"
#include "lab_m1/Tema2/enemy3D.h"

namespace m1
{
    class Maze
    {
    public:
        int size;
        std::vector<std::vector<int>> cells;
        std::vector<std::vector<int>> visited;
        std::vector<Enemy3d*> enemies;
        int totalEnemies;
        std::vector<std::pair<float, float>> exits;
        int numberOfExits;

        Maze(int size);
        ~Maze();

        void generate();
        void print();
        void printExits();

    private:
        void set();
        void createExits();
        void createEnemies();
    };
}   // namespace m1