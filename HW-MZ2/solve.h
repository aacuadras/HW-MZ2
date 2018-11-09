
#ifndef SOLVE_H
#define SOLVE_H

#include <string>
#include <unordered_set>
#include "minpriorityqueue.h" // Includes <vector>, <unordered_map>, <utility>
#include "Graph.h"

using namespace std;

// For the mandatory running time, assume that the time for
// operations of queue, unordered_set, and map are O(1). 
// (They are for average-case, but not worst-case).
//
// For the mandatory running time below, s is the length of 
// the input string representing the maze.
// 
// For a complete description of the maze string 
// and maze solution formats, see the assignment pdf.
//
//
// Returns a string representing a shortest solution to the maze.
// Undefined behavior if the maze is not valid or has no solution.
//
// Must run in O(s*log(s)) time.
string solve(string maze)
{
	Graph m;
	int row = 0;
	int	column = 0;
	int	numCols = 0;
	vector<int> cost;
	vector<int> coordinates;

	//Step 1: Construct the graph and consider the portals as normal vertices 
	for (int i = 0; i < maze.length(); i++)
	{
		if (maze[i] >= 48 && maze[i] <= 57)
		{
			m.addVertex(row, column);
			m.markPortal(row, column, (maze[i] - 48));
			column++;
			if (row == 0)
				numCols++;
		}
		else if (maze[i] == ' ')
		{
			m.addVertex(row, column);
			column++;
			if (row == 0)
				numCols++;
		}
		else if (maze[i] == '#')
		{
			column++;
			if (row == 0)
				numCols++;
		}
		else if (maze[i] == '\n')
		{
			row++;
			column = 0;
			if (row == 0)
				numCols++;
		}
	}
	//Once the graph is constructed, all the vertices have to be connected with cost 1, even if the vertex is a portal
	//Now we call this function to connect the portals to one another with their corresponding weights
	m.connectPortals();
}

#endif 

