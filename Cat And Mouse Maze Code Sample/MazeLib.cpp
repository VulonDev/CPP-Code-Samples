//MazeLib.cpp: Implementation for Cat, Mouse, Node, and Maze structs
#include "MazeLib.h"

//Node constructor and functions
Node::Node(int n) {
	id = n;
}

//Cat constructor and functions
Cat::Cat(Node* start) {
	curr = start;
	prev = nullptr;
}

/*
* The cat moves one node at a time
* If the current node contains a mouse scent trail, investigate a random adjacent node to check if it also has a trail
* if there is a trail on that investigated node, and the trail is stronger there, move there
* Otherwise select a random other neighbor node to move to
* Do not backtrack to the node the cat just came from unless there is no other choice
*/
void Cat::Move(const vector<Node*>& scentTrail) {
	if (find(scentTrail.begin(), scentTrail.end(), curr) != scentTrail.end()) {
		cout << "CAT detects a trail on node " + to_string(curr->id);
		Node* investigate = curr->adj[rand() % curr->adj.size()];
		cout << " and investigates node " + to_string(investigate->id) + " in search of a stronger scent.";
		bool followedTrail = false;
		for (int i = scentTrail.size() - 1; i > 0; i--) {
			if (scentTrail[i] == curr) {
				for (int j = i - 1; j >= 0; j--) {
					if (scentTrail[j] == investigate) {
						cout << " Succeeding, CAT follows it";
						followedTrail = true;
						prev = curr;
						curr = investigate;
						break;
					}
				}
				break;
			}
		}
		if(!followedTrail) {
			cout << " Failing, CAT instead moves";
			Node* next;
			do {
				next = curr->adj[rand() % curr->adj.size()];
			} while (curr->adj.size() > 1 && (next == investigate || (next == prev && curr->adj.size() > 2)));
			prev = curr;
			curr = next;
		}
	}
	else {
		cout << "CAT moves from node " + to_string(curr->id);
		Node* next;
		do {
			next = curr->adj[rand() % curr->adj.size()];
		} while (next == prev && curr->adj.size() > 1);
		prev = curr;
		curr = next;
	}
	cout << " to node " + to_string(curr->id) << endl;
}

//Mouse constructor and functions

Mouse::Mouse(Node* start) {
	scentTrail = vector<Node*>(3, nullptr);
	scentTrail[0] = start;
}

/*
* As the mouse moves track its scent trail, including the last two nodes the mouse visited, and its current node
* Move one node at a time
* Check if there is a cat on an adjacent node. If there is a cat, calculate which node is the most direct route to the exit and go there
* If there is not a cat, wander randomly to an adjacent node
* Do not backtrack to the most node the mouse just left
*/

void Mouse::Move(Node* cat, Maze& maze) {
	scentTrail[2] = scentTrail[1];
	scentTrail[1] = scentTrail[0];

	bool panic = false;

	for (Node* neighbor : scentTrail[0]->adj) {
		if (neighbor == cat) {
			panic = true;
			cout << "MOUSE PANICS after detecting CAT on a neighbor node and runs for the exit!" << endl;
			break;
		}
	}

	

	cout << "MOUSE moves from node " + to_string(scentTrail[0]->id);

	if (panic) {
		Node* toExit = maze.FindMoveDirection(scentTrail[0], maze.exit);
		if (toExit == nullptr) {
			Node* next;
			do {
				next = scentTrail[0]->adj[rand() % scentTrail[0]->adj.size()];
			} while (next == scentTrail[2] && scentTrail[0]->adj.size() > 1);
			scentTrail[0] = next;
		}
		else {
			scentTrail[0] = toExit;
		}
	}
	else {
		Node* next;
		do {
			next = scentTrail[0]->adj[rand() % scentTrail[0]->adj.size()];
		} while (next == scentTrail[2] && scentTrail[0]->adj.size() > 1);
		scentTrail[0] = next;
	}
	cout << " to node " + to_string(scentTrail[0]->id) << endl;
}

//Maze constructor and functions

Maze::Maze(int s) {
	size = s;
	if (size < 3) {
		cerr << "Invalid maze size. Size must be at least 3. Automatically Correcting." << endl;
		size = 3;
	}

	allNodes.reserve(size);
	for (int i = 0; i < size; ++i) {
		allNodes.push_back(new Node(i));
	}

	start = allNodes[0];
	exit = allNodes[size - 1];

	vector<bool> visited(size, false);
	vector<int> edgeCount(size, 0);

	CreateMaze(start, visited, edgeCount);
	EnsureMinOneEdge(edgeCount);
}


void Maze::AddEdge(Node* from, Node* to) {
	from->adj.push_back(to);
	to->adj.push_back(from);
}


/*
* A recursive function to generate a maze
* Constraints upon the maze:
* The start and exit nodes may not be directly linked by an edge
* Each node may not have more than four edges and must have at least one edgeh
* The total number of nodes is to be specified by the size property
* Looping paths can be created back to prior nodes, except the exit and start nodes. 
* There is a 10% chance for an edge to be created back to a prior visited node, provided that node satisfies the above constraints
*/

void Maze::CreateMaze(Node* curr, vector<bool>& visited, vector<int>& edgeCount) {
	visited[curr->id] = true;
	vector<int> directions(size - curr->id - 1);
	iota(directions.begin(), directions.end(), 1);
	random_shuffle(directions.begin(), directions.end());

	for (int dir : directions) {
		int nextIndex = curr->id + dir;
		if (nextIndex < size && edgeCount[curr->id] < 4 && edgeCount[nextIndex] < 4 &&
			(!visited[nextIndex] || (static_cast<double>(rand()) / RAND_MAX < 0.1)) &&
			!(curr->id == 0 && nextIndex == size - 1)){
			AddEdge(curr, allNodes[nextIndex]);
			edgeCount[curr->id]++;
			edgeCount[nextIndex]++;
			if (!visited[nextIndex]) {
				CreateMaze(allNodes[nextIndex], visited, edgeCount);
			}
		}
	}
}

//Post processing step after inital maze generation. Ensures every node has at least one edge
void Maze::EnsureMinOneEdge(vector<int>& edgeCount) {
	for (int i = 0; i < size; ++i) {
		if (edgeCount[i] == 0) {
			for (int j = 1; j < size; ++j) {
				int neighborIndex = (i + j) % size;
				if (edgeCount[neighborIndex] < 4 && neighborIndex != i && !(i == 0 && neighborIndex == size - 1)) {
					AddEdge(allNodes[i], allNodes[neighborIndex]);
					edgeCount[i]++;
					edgeCount[neighborIndex]++;
					break;
				}
			}
		}
	}
}

// Uses BFS to find the shortest route from start to end, and returns the first node in that direction from start
Node* Maze::FindMoveDirection(Node* start, Node* end) {
	if (start == end) {
		return nullptr; // No movement needed if start and end are the same
	}

	std::vector<Node*> prev(size, nullptr); // To store the path
	std::vector<bool> visited(size, false);
	std::queue<Node*> queue;

	queue.push(start);
	visited[start->id] = true;

	// BFS to find the shortest path
	while (!queue.empty()) {
		Node* current = queue.front();
		queue.pop();

		for (Node* neighbor : current->adj) {
			if (!visited[neighbor->id]) {
				prev[neighbor->id] = current;
				queue.push(neighbor);
				visited[neighbor->id] = true;

				if (neighbor == end) {
					// Path found, backtrack to find the first move
					Node* move = end;
					while (prev[move->id] != start) {
						move = prev[move->id];
					}
					return move;
				}
			}
		}
	}

	return nullptr; // Return nullptr if no path is found
}

//Prints a representation of the maze as a list of each node and the nodes it is connected to
void Maze::PrintMaze() {
	for (int i = 0; i < allNodes.size(); i++) {
		if (allNodes[i] == start) {
			cout << "[START] ";
		}
		else if (allNodes[i] == exit) {
			cout << "[EXIT] ";
		}
		cout << "Node " + to_string(allNodes[i]->id); 
		cout << " connects to:\n";
		for (int j = 0; j < allNodes[i]->adj.size(); j++) {
			cout << to_string(allNodes[i]->adj[j]->id) << endl;
		}
		cout << endl;
	}
}
