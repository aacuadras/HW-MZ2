
#ifndef MINPRIORITYQUEUE_H
#define MINPRIORITYQUEUE_H

// NOTE: You may not include any other libraries!
#include <unordered_map>
#include <vector>
#include <utility> // Has pair and swap

using namespace std;

template <typename T>
class MinPriorityQueue
{
	// For the mandatory running times below:
	//
	// n is the number of strings in the MinPriorityQueue.
	//
	// Assume the operations of map are O(1) time 
	// (they are average case, but not worst-case).

	public:
		// Creates an empty MinPriorityQueue
		MinPriorityQueue()
		{
			
		}

		// Returns the number of elements in the MinPriorityQueue.
		//
		// Must run in O(1) time.
		int size()
		{
			return H.size();
		}	

		// Pushes a new element x with priority p
		// into the MinPriorityQueue.
		//
		// Must run in O(log(n)) time. 
		void push(T x, int p)
		{
			pair<T, int> baby;
			baby.first = x;
			baby.second = p;
			H.push_back(baby);
			I[x] = H.size() - 1;

			int i = H.size() - 1;
			while (violation(i))
			{
				swap(I[H[i].first], I[H[parent(i)].first]);
				swap(H[i], H[parent(i)]);
				i = parent(i);
			}
		}

		// Returns the string at the front of the MinPriorityQueue.
		// Undefined behavior if the MinPriorityQueue is empty.
		// 
		// Must run in O(1) time.
		T front()
		{
			return H.front().first;
		}

		// Removes the string at the front of the MinPriorityQueue.
		// Undefined behavior if the MinPriorityQueue is empty.
		//
		// Must run in O(log(n)) time. 
		void pop()
		{
			//Extra step: Check if the vector is empty, if it is: do nothing.
			if (H.empty())
				return;

			//Step 1: Swap the minimum value with the last one inserted
			swap(I[H[0].first], I[H[H.size() - 1].first]);
			swap(H[0], H[H.size() - 1]);

			//Step 2: Delete the now last value
			I.erase(H[H.size() - 1].first);
			H.pop_back();

			//Step 3: Check if there is a violation from top to bottom
			int i = 0;
			while (pop_violation(i))
			{
				int minIndex = getMinChildren(i);
				swap(I[H[i].first], I[H[minIndex].first]);
				swap(H[i], H[minIndex]);
				i = minIndex;
			}
		}

		// If s is in the MinPriorityQueue 
		// with current priority at least new_p,  
		// then changes the priority of x to new_p.
		// Undefined behavior otherwise.
		//
		// Must run in O(log(n)) time. 
		void decrease_key(T x, int new_p)
		{
			//Step 1: Find the index of the object and decrease the key in the vector
			H[I[x]].second = new_p;

			//Step 2: Check for violations in the children, if the "node" is a leaf, do nothing
			int i = I[x];
			while (pop_violation(i))
			{
				int minIndex = getMinChildren(i);
				swap(I[H[i].first], I[H[minIndex].first]);
				swap(H[i], H[minIndex]);
				i = minIndex;
			}

			//Step 3: Check for violations upwards
			i = I[x];
			while (violation(i))
			{
				swap(I[H[i].first], I[H[parent(i)].first]);
				swap(H[i], H[parent(i)]);
				i = parent(i);
			}
		}

	private:
		// You don't need any other instance variables,
		// but you can add some if you want to.
		unordered_map<T, int> I; // Maps elements to indices in H.
		vector< pair<T, int> > H;     // The array containing the heap.

		///Helper functions
		bool violation(int index)
		{
			if (H[index].second < H[parent(index)].second)
				return true;
			else
				return false;
		}

		int parent(int index)
		{
			return (index - 1) / 2;
		}

		///Pop helper functions
		int left_children(int index)
		{
			return (index * 2) + 1;
		}

		int right_children(int index)
		{
			return (index * 2) + 2;
		}

		bool pop_violation(int index)
		{
			try
			{
				if (H.at(left_children(index)).second < H.at(index).second)
					return true;
			}
			catch (const out_of_range& oor) {

			}
			try
			{
				if (H.at(right_children(index)).second < H.at(index).second)
					return true;
			}
			catch (const out_of_range& oor1) {

			}
			return false;
		}

		int getMinChildren(int index)
		{
			//Try to access the left children and see if an exception occurs
			try
			{
				int test = H.at(left_children(index)).second;
			}
			catch (const out_of_range& oor) {
				return right_children(index);
			}
			try
			{
				int test2 = H.at(right_children(index)).second;
			}
			catch (const out_of_range& oor1) {
				return left_children(index);
			}
			if (H[left_children(index)].second < H[right_children(index)].second)
				return left_children(index);
			else
				return right_children(index);
		}
};

#endif 

