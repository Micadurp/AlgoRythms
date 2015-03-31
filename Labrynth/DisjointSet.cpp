#include "DisjointSet.h"

DisjointSet::DisjointSet(bool useRanks, bool useCompression)
{
	capacity = 0;
	nrOfElements = 0;
	setCount = 0;
	usePathCompression = useCompression;
	useRank = useRanks;
}

DisjointSet::DisjointSet(int size, bool useRanks, bool useCompression)
{

	capacity = 0;
	nrOfElements = 0;
	setCount = 0;
	usePathCompression = useCompression;
	useRank = useRanks;

	if (size <= 0)
		return;

	set = new Node*[this->capacity];

}
DisjointSet::~DisjointSet()
{
	for (int i = 0; i < this->nrOfElements; i++)
		delete set[i];
	delete[] set;
}

void DisjointSet::Add(int indexID)
{
	Expand(1);

	set[nrOfElements] = new Node(indexID);
	nrOfElements++;
	setCount++;
}
int DisjointSet::AddRange(int size)
{
	if (size < 1)
		return -1;

	Expand(size);

	int first = nrOfElements;
	int toAdd = nrOfElements + size;

	for (int i = nrOfElements; i < toAdd; i++)
	{
		set[i] = new Node(i);
		nrOfElements++;
		setCount++;
	}

	return first;
}

void DisjointSet::Union(int a, int b)
{
	int aLocation = Find(a);
	int bLocation = Find(b);

	if (aLocation < 0 || bLocation < 0)
		return;

	if (aLocation != bLocation)
		Merge(set[aLocation]->id, set[bLocation]->id);
}
int	DisjointSet::Find(int indexID)
{
	int root = -1;

	if (!this->setCount)
		return root;

	if (this->usePathCompression)
	{
		if (indexID != set[indexID]->parent->id)
			set[indexID]->parent = set[Find(set[indexID]->parent->id)];
		root = set[indexID]->parent->id;
	}
	else
	{
		int i = indexID;

		while (set[i]->parent->id != i)
			i = set[i]->parent->id;
		root = i;
	}

	return root;
}

DisjointSet::Node** DisjointSet::getSetCollection()
{
	return set;
}
int	DisjointSet::getCapacity() const
{
	return capacity;
}
int	DisjointSet::getSetCount() const
{
	return setCount;
}

void DisjointSet::Merge(int a, int b)
{
	if (setCount == 1)
		return;

	if (useRank)
	{
		// A -> B
		if (set[a]->rank < set[b]->rank)
			set[a]->parent = set[b];
		// B -> A
		else
		{
			set[b]->parent = set[a];

			//Increment rank if equal
			if (set[a]->rank == set[b]->rank)
				set[a]->rank++;
		}
	}
	else
	{
		set[a]->parent = set[b];
	}
	setCount--;
}

void DisjointSet::Expand(int toAdd)
{
	int cap = nrOfElements + toAdd;

	if (cap < capacity)
		return;

	capacity = cap;
	Node** temp = new Node*[capacity];

	for (int i = 0; i< nrOfElements; i++)
		temp[i] = set[i];

	delete[] set;
	set = temp;
}
