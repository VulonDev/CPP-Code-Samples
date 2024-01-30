//MazeLib.h: Header file for Cat, Mouse, Node, and Maze structs

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <random> //for rand, srand
#include <numeric> //for iota
#include <algorithm> //for random_shuffle


using namespace std;


/*
A node represents one 'room' in the maze
id = the numerical id of a node within a maze where id >= 0 && id < maze size
adj = pointers to all nodes a node is connected to (edges)
*/
struct Node {
	int id;
	vector<Node*> adj;

	Node(int n);
};

/*
* A maze is represented as an undirected graph data structure
* the maze is generated with a modified version of DFS which satisfies a number of constraints on the maze's layout
* size = the # of nodes in the maze
* start = the node that is the beginning of the maze
* exit = the node that is the end of the maze
* allNodes = a list of pointers to every node in the maze
* AddEdge() = adds an edge between two nodes within a maze
* FindMoveDirection() = runs BFS on the maze to find the quickest route between two nodes, and then returns the first move on that route
* the Mouse uses FindMoveDirection() to figure out which node it should move to next if the cat is nearby
* CreateMaze = a recursive function to generate a maze, called by the constructor, using DFS to generate while also satisfying constraints on valid edges within the maze
* EnsureMinOneEdge() = a post-processing step after CreateMaze() to make sure each node connects to at least one other.
*/
struct Maze {
	int size;
	Node* start;
	Node* exit;
	vector <Node* > allNodes;

	void AddEdge(Node* from, Node* to);

	Node* FindMoveDirection(Node* start, Node* end);
	void PrintMaze();
	void CreateMaze(Node* curr, vector<bool>& visited, vector<int>& edgeCount);
	void EnsureMinOneEdge(vector<int>& edgeCount);

	Maze(int s);


};

/*
* A cat object chases a mouse object through a maze
* The cat will search for the mouse's scent trail, representing nodes the mouse has recently visited
* If the cat ends its movement on the same node as the mouse, the mouse is caught and killed. The cat wins.
* The cat can check if the cat's current node is present in the mouse's scent trail
* If the cat's current node contains a scent trail, the cat should investigate one randomly selected adjacent node during its move.
* If that investigated node contains a stronger scent trail than the current node, the cat should move to that node.
* Otherwise the cat should move to a different randomly selected adjacent node.
* The cat will not backtrack to the node it just left unless it has no other option.
* curr = the node the cat currently occupies
* Move() = calculates which node the cat moves to next. The cat always moves one node.
*/
struct Cat {
	Node* curr;
	Node* prev;

	void Move(const vector<Node*>& scentTrail);

	Cat(Node* start);
};
/*
* A mouse object wanders through a maze, selecting random nodes to travel to.
* The mouse will not backtrack to the node it just left.
* If a mouse object detects a cat object on an adjacent node, the mouse panics
* If a mouse is panicked, it calculates the fastest route to the maze exit and moves deliberately towards the fastest route to the exit
* A mouse leaves a scent trail for the cat to follow on the last two nodes it has traversed. Lower indices in the scentTrail array represent a stronger scent trail.
* If the mouse enters a node currently occupied by the cat, the mouse is caught and killed and the cat wins.
* If the mouse ends its movement on the exit node, the mouse escapes and the mouse wins.
* scentTrail = current node and the last two nodes the mouse has occupied
* Move() = calculates the mouse's next move. The mouse always moves one node.
*/
struct Mouse {
	vector<Node*> scentTrail; //scentTrail[0] = current node, scentTrail[1] = previous node, scentTrail[2] = node prior to previous node

	void Move(Node* cat, Maze& maze);
	Mouse(Node* start);
};


