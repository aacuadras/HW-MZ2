
#ifndef SOLVE_H
#define SOLVE_H

#include <string>
#include <unordered_set>
#include "minpriorityqueue.h" // Includes <vector>, <unordered_map>, <utility>
#include"WeightedGraph.h"

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
	WeightedGraph m;
	int row = 0;
	int	column = 0;
	int	numCols = 0;
	vector<int> portals;
	vector<int> cost;

	//Step 1: Construct the graph
	for (int i = 0; i < maze.length(); i++)
	{
		if (maze[i] >= 48 && maze[i] <= 57)
		{
			//Enters the data of the portals to two vectors, the first one stores the location in two indexes 
			//and the other one stores the costs 
			portals.push_back(row);
			portals.push_back(column);
			portals.push_back(maze[i] - 48);
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
	m.setDimensions(row, numCols);
	//This functions sends the two vectors and makes them as normal vertices, but with special properties
	m.buildPortals(portals, cost);
	m.buildEdges();
	//m.testDisplay();

	return m.shortest_path(m.getStart(), m.getEnd(), maze);
}

#endif 

