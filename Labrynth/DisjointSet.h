#ifndef DISJOINTSET_H
#define DISJOINSET_H

#include <vector>
using namespace std;

class DisjointSet 
{
public:

	class Node
	{
	public:
		Node* parent;
		int id;
		int rank;

		Node(int i)
			:id(i), rank(0)
		{
			this->parent = this;
		}
	};


private:
	Node** set;
	int capacity;
	int nrOfElements;
	int setCount;

	bool usePathCompression;
	bool useRank;


private:
	// Expands if needed 
	void Expand(int toAdd);
	// Merges the two sets 
	void Merge(int, int);

public:
	DisjointSet(bool useRank = true, bool useCompression = true);
	DisjointSet(int size, bool useRank = true, bool useCompression = true);
	virtual~DisjointSet();


	void	Add(int indexID);
	int		AddRange(int size);
	void	Union(int a, int b);
	int		Find(int indexID);


	// Returns a pointer to an array of DosjointSet pointers of type T. (Use Capacity() to get element count)
	Node** getSetCollection();

	// Returns the current number of elements in the set
	int getCapacity() const;

	// Return the number of uniuqe sets in the collection 
	int getSetCount() const;
};

#endif