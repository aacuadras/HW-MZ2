#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<string>
#include"vertex.h"
using namespace std;

class Graph
{
private:
	///////////////////////////////////////////
	//List of all the vertices
	unordered_map<string, Vertex*> vertexIndex;
	//"Dictionary" of every vertex and it's "breadcrumb"
	unordered_map<Vertex*, Vertex*> breadcrumbs;
	//Map for the portals
	unordered_map<int, vector<int>> portals;
	//Vector to keep track of the costs
	vector<int> portalIndexes;
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

	//Checks if the cost is in the portalIndexes vector and returns a boolean value
	bool checkCost(int cost)
	{
		for (int i = 0; i < portalIndexes.size(); i++)
			if (portalIndexes[i] == cost)
				return true;
		return false;
	}

public:

	//Adds a normal vertex to the graph
	void addVertex(int rows, int columns, int cost)
	{
		string reference = to_string(rows) + "," + to_string(columns);
		Vertex* baby = new Vertex(rows, columns);
		vertexIndex[reference] = baby;
	}

	//Checks the neighbors up, down, left and right and adds them to the neighs list in the vertex class
	///This function will check for normal edges, it will not connect to a portal
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

	///TODO
	//Adds the portal indexes to the map and the key is the cost, the vector "portalIndexes" keeps track of the indexes
	void markPortal(int row, int column, int cost)
	{
		if (portalIndexes.empty())
			portalIndexes.push_back(cost);
		//Checks if the cost is in the portalIndexes vector and returns a boolean value
		else if (checkCost(cost))
		{
			//If the cost is already in the vector, do nothing
		}
		else
			portalIndexes.push_back(cost);

		portals[cost].push_back(row);
		portals[cost].push_back(column);
	}
};
