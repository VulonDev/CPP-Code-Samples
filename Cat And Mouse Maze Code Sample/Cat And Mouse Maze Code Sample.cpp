// Cat And Mouse Maze Code Sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MazeLib.h"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int catWins = 0;
    int mouseWins = 0;

    int simSize = 10; //How many cycles are run
    int mazeSize = 30; //How big each maze is

    
    for (int i = 0; i < simSize; i++) {
        Maze maze = Maze(mazeSize);
        Mouse mouse = Mouse(maze.start);
        Cat cat = Cat(nullptr);
        maze.PrintMaze();

        mouse.Move(cat.curr, maze);
        mouse.Move(cat.curr, maze);

        cout << "A hungry CAT appears on node 0!" << endl;
        cat.curr = maze.start;

        while (true) {
            mouse.Move(cat.curr, maze);
            cat.Move(mouse.scentTrail);
            if (cat.curr == mouse.scentTrail[0]) {
                cout << "THE CAT CATCHES THE MOUSE!\n" << endl;
                catWins++;
                break;
            }
            if (mouse.scentTrail[0] == maze.exit) {
                cout << "THE MOUSE ESCAPES!\n" << endl;
                mouseWins++;
                break;
            }
        }

    }

    cout << "\n\nWith " + to_string(simSize) + " trials using maze size " + to_string(mazeSize) + ":" << endl;
    cout << "Cat wins: " + to_string(catWins) << endl;
    cout << "Mouse wins: " + to_string(mouseWins) << endl;

}