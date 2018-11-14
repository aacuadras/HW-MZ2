#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<string>
#include<iostream>
#include"vertex.h"
#include "minpriorityqueue.h"
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
	//Used to find the the Weight between Edges
	unordered_map< pair<Vertex*, Vertex*>, int> EdgeWeight;
	//Weight of a vertex
	unordered_map<Vertex*, int> VertexWeight;

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
	void addNeighbor(string index, Vertex*& x, Vertex *&y, int cost)
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
				newN.second = cost;
				x->neighs.push_back(newN);
			}
			if (!foundy)
			{
				newN.first = x;
				newN.second = cost;
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

	void Dijkstras(string s)
	{
		MinPriorityQueue<Vertex*> AwesomeSauce;

		for (auto x : vertexIndex)
		{
			VertexWeight[vertexIndex[s]] = INFINITY;
			AwesomeSauce.push(vertexIndex[s], INFINITY);
		}

		AwesomeSauce.decrease_key(getStart(), 0);
		VertexWeight[getStart()] = 0;

		while (AwesomeSauce.size() > 0)
		{
			Vertex *current;
			current = new Vertex(AwesomeSauce.front);
			AwesomeSauce.pop();

			for(int y = 0; y < current->neighs.size(); y++)
			{
				relax(current, current->neighs[y].first);
				
			}
		}
	}
	void relax(Vertex *a, Vertex *b)
	{
		pair<Vertex*, Vertex*> obj;
		obj.first = a;
		obj.second = b;

		if (VertexWeight[a] + EdgeWeight[obj] < VertexWeight[b])
		{
			VertexWeight[b] = VertexWeight[a] + EdgeWeight[obj];
			breadcrumbs[b] = a;
		}
	}

public:

	//Adds a normal vertex to the graph
	void addVertex(int rows, int columns)
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
			addNeighbor(neighbor_check, x, y, 1);
			//Check down
			neighbor_check = to_string((x->row + 1)) + "," + to_string(x->col);
			addNeighbor(neighbor_check, x, y, 1);
			//Check left
			neighbor_check = to_string(x->row) + "," + to_string((x->col - 1));
			addNeighbor(neighbor_check, x, y, 1);
			//Check right
			neighbor_check = to_string(x->row) + "," + to_string((x->col + 1));
			addNeighbor(neighbor_check, x, y, 1);
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

	//Connects the portals as neighbors of one another
	void connectPortals()
	{
		string first_portal;
		string second_portal;
		Vertex * y;
		for (int i = 0; i < portalIndexes.size(); i++)
		{
			first_portal = "";
			second_portal = "";
			first_portal = first_portal + to_string(portals[portalIndexes[i]].at(0)) + "," + to_string(portals[portalIndexes[i]].at(1));
			second_portal = second_portal + to_string(portals[portalIndexes[i]].at(2)) + "," + to_string(portals[portalIndexes[i]].at(3));
			addNeighbor(second_portal, vertexIndex[first_portal], y, portalIndexes[i]);
			addNeighbor(first_portal, vertexIndex[second_portal], y, portalIndexes[i]);
		}
	}

	//Method used to display the graph, debugging purposes
	void testDisplay()
	{
		system("cls");
		system("cls");
		int counter = 1;
		string current;

		for (auto i : vertexIndex)
		{
			cout << counter << ": " << i.first << endl;
			cout << "Neighbors: ";
			for (int k = 0; k < i.second->neighs.size(); k++)
			{
				current = to_string(i.second->neighs[k].first->row) + "," + to_string(i.second->neighs[k].first->col);
				cout << current << " (Cost: " << i.second->neighs[k].second << ") " << " :: ";
			}
			counter++;
			cout << endl << endl;
		}
		cout << "NumRows: " << numRows << endl;
		cout << "NumColumns: " << numColumns << endl;
		current = to_string(start->row) + "," + to_string(start->col);
		cout << "Start: " << current << endl;
		current = to_string(end->row) + "," + to_string(end->col);
		cout << "End: " << current << endl << endl;
		system("pause");
	}


};
