#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<string>
#include"vertex.h"
using namespace std;

///Notes
//This functions as an unweighted graph
//Vertex.neighs changed to a pair, the int represents the steps necessary to move to the other location


class WeightedGraph
{
private:
	///////////////////////////////////////////
	//List of all the vertices
	unordered_map<string, Vertex*> vertexIndex;
	//"Dictionary" of every vertex and it's "breadcrumb"
	unordered_map<Vertex*, Vertex*> breadcrumbs;
	//Start point for the maze
	Vertex* start;
	//End point for the maze
	Vertex* end;
	//Dimensions of the maze: rows
	int numRows;
	//Dimensions of the maze: columns
	int numColumns;
	////////////////////////////////////////////

	//Helper method to find the startpoint of the maze
	void setStart()
	{
		for (auto i : vertexIndex)
		{
			//Sets the start variable, searches for a space in the first or last row/column
			if (i.second->row == 0 || i.second->row == (numRows - 1))
				start = i.second;
			else if (i.second->col == 0 || i.second->col == (numColumns - 1))
				start = i.second;
		}

	}
	//Helper method to find the endpoint of the maze
	void setEnd()
	{
		for (auto i : vertexIndex)
		{
			//Sets the end variable, searches for a space int the first or last row/column that doesn't equals the start variable 
			if ((i.second->row == 0 || i.second->row == (numRows - 1)) && i.second != start)
				end = i.second;
			else if ((i.second->col == 0 || i.second->col == (numColumns - 1)) && i.second != start)
				end = i.second;
		}
	}

	//Helper method to add the neighbors
	//Checks if the neighbors are already in the list and if they aren't, the new neighbor is added
	//Since this is adding a normal neighbor without portals, the cost is 1
	void addNeighbor(string index, Vertex*& x, Vertex *&y)
	{
		pair<Vertex*, int> newN;
		bool foundx = false;
		bool foundy = false;
		try {
			y = vertexIndex.at(index);
			for (int i = 0; i < x->neighs.size(); i++)
				if (x->neighs[i].first == y)
					foundx = true;
			for (int k = 0; k < y->neighs.size(); k++)
				if (y->neighs[k].first == x)
					foundy = true;

			if (!foundx)
			{
				newN.first = y;
				newN.second = 1;
				x->neighs.push_back(newN);
			}
			if (!foundy)
			{
				newN.first = x;
				newN.second = 1;
				y->neighs.push_back(newN);
			}
		}
		catch (out_of_range oor)
		{
			index = "";
		}
	}

	//Checks the shortest path to the end of the maze
	///Doesn't work for this HW
	/*
	void breadthFirstSearch(Vertex* s)
	{
		queue<Vertex*> Q;
		unordered_set<Vertex*> marked;

		marked.insert(s);
		Q.push(s);
		while (!Q.empty())
		{
			Vertex* x = Q.front();
			Q.pop();
			for (int i = 0; i < x->neighs.size(); i++)
			{
				Vertex* y = x->neighs[i].first;
				if (marked.find(y) == marked.end())
				{
					marked.insert(y);
					Q.push(y);
					breadcrumbs[y] = x;
				}
			}
		}
	}
	*/

	bool find(vector<Vertex*> V, int r, int c)
	{
		for (int i = 0; i < V.size(); i++)
		{
			if (V[i]->row == r && V[i]->col == c)
				return true;
		}
		return false;
	}

public:

	//Creates a new vertex and sets the key as the coordinates as follows: r,c
	void addVertex(int rows, int columns)
	{
		string reference = to_string(rows) + "," + to_string(columns);
		Vertex* baby = new Vertex(rows, columns);
		vertexIndex[reference] = baby;
	}

	void buildPortals(vector<int> coordinates, vector<int> cost)
	{
		//The cost has to be repeated in the vector, so search for the duplicate and their coordinates
		//Searching for coordinates is as follows...
		//Row: (cost * 2)
		//Column: (cost * 2) + 1
		//The special property is that the two vertices have to be neighbors and have the cost of the ASCII number they were passed
		//and that is in the cost vector already
		unordered_map<int, string> portals;
		for (int i = 0; i < cost.size(); i++)
		{
			//This will return true when the first part of the portal is passed
			//':' is the separator between the two coordinates of the portal
			if (portals[cost[i]].empty())
			{
				portals[cost[i]] = to_string(coordinates[cost[i] * 2]) + "," + to_string(coordinates[(cost[i] * 2) + 1]);
			}
			else
			{
				portals[cost[i]] += ":" + to_string(coordinates[cost[i] * 2]) + "," + to_string(coordinates[(cost[i] * 2) + 1]);
			}
		}


	}

	//Checks the neighbors up, down, left and right and adds them to the neighs list in the vertex class
	///Since neighs became a vector of pairs, we have to set the cost of every neighbor too, so we have to check
	///if the neighbor is a portal and where it leads to. Check addNeighbor for more information.
	void buildEdges()
	{
		Vertex* x;
		Vertex* y;
		string neighbor_check;

		for (auto i : vertexIndex)
		{
			x = i.second;
			//Check up
			neighbor_check = to_string((x->row - 1)) + "," + to_string(x->col);
			addNeighbor(neighbor_check, x, y);
			//Check down
			neighbor_check = to_string((x->row + 1)) + "," + to_string(x->col);
			addNeighbor(neighbor_check, x, y);
			//Check left
			neighbor_check = to_string(x->row) + "," + to_string((x->col - 1));
			addNeighbor(neighbor_check, x, y);
			//Check right
			neighbor_check = to_string(x->row) + "," + to_string((x->col + 1));
		}
		setStart();
		setEnd();
		/*
		cout << "Graph constructed!" << endl;
		system("pause");
		*/
	}

	//Sets the number of columns and rows that the maze has
	void setDimensions(int nRows, int nCols)
	{
		numRows = nRows;
		numColumns = nCols;
	}

	//Methods declared to make a less confusing code
	Vertex* getStart()
	{
		return start;
	}
	Vertex* getEnd()
	{
		return end;
	}

	//Uses breadthFirstSearch to set the breadcrumbs and set the shortest path
	///Now, instead of using breadthFirstSearch, we have to use Dijkstra's algorithm to set the path
	///with the shortest cost
	string shortest_path(Vertex* s, Vertex* e, string maze)
	{
		/*
		breadthFirstSearch(s);
		Vertex* current = e;
		string result;
		vector<Vertex*> solution;
		int rows = 0;
		int columns = 0;

		while (current != s)
		{
			solution.push_back(current);
			current = breadcrumbs[current];
		}
		solution.push_back(current);

		for (int i = 0; i < maze.length(); i++)
		{
			if (maze[i] == '#')
			{
				result += maze[i];
				columns++;
			}
			else if (maze[i] == '\n')
			{
				result += maze[i];
				rows++;
				columns = 0;
			}
			else if (maze[i] == ' ')
			{
				//Have to check the result of the shortest path to see if it should be reformated
				if (find(solution, rows, columns))
				{
					result += 'o';
				}
				else
					result += maze[i];
				columns++;
			}
		}
		return result;
		*/
	}
};

#endif

