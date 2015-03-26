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

	this->set = new Node*[this->capacity];
	for (int i = 0; i < size; i++)
		this->set[i] = NULL;

}
DisjointSet::~DisjointSet()
{
	for (int i = 0; i < this->nrOfElements; i++)
		delete this->set[i];
	delete[] this->set;
}

void DisjointSet::Add(int indexID)
{
	Expand(1);

	this->set[this->nrOfElements] = new Node(indexID);
	this->nrOfElements++;
	this->setCount++;
}
int DisjointSet::AddRange(int size)
{
	if (size < 1)
		return -1;

	Expand(size);

	int first = this->nrOfElements;
	int toAdd = this->nrOfElements + size;

	for (int i = this->nrOfElements; i < toAdd; i++)
	{
		this->set[i] = new Node(i);
		this->nrOfElements++;
		this->setCount++;
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
		Merge(this->set[aLocation]->id, this->set[bLocation]->id);
}
int	DisjointSet::Find(int indexID)
{
	int root = -1;

	if (!this->setCount)
		return root;

	if (this->usePathCompression)
	{
		if (indexID != this->set[indexID]->parent->id)
			this->set[indexID]->parent = this->set[Find(this->set[indexID]->parent->id)];
		root = this->set[indexID]->parent->id;
	}
	else
	{
		int i = indexID;

		while (this->set[i]->parent->id != i)
			i = this->set[i]->parent->id;
		root = i;
	}

	return root;
}

DisjointSet::Node** DisjointSet::getSetCollection()
{
	return this->set;
}
int	DisjointSet::getCapacity() const
{
	return (int)this->capacity;
}
int	DisjointSet::getSetCount() const
{
	return this->setCount;
}

void DisjointSet::Merge(int a, int b)
{
	if (this->setCount == 1)
		return;

	if (this->useRank)
	{
		// A -> B
		if (this->set[a]->rank < this->set[b]->rank)
			this->set[a]->parent = this->set[b];
		// B -> A
		else
		{
			this->set[b]->parent = this->set[a];

			//Increment rank if equal
			if (this->set[a]->rank == this->set[b]->rank)
				this->set[a]->rank++;
		}
	}
	else
	{
		this->set[a]->parent = this->set[b];
	}
	this->setCount--;
}

void DisjointSet::Expand(int toAdd)
{
	int cap = this->nrOfElements + toAdd;

	if (cap < this->capacity)
		return;

	this->capacity = cap;
	Node** temp = new Node*[this->capacity];

	for (int i = 0; i< this->nrOfElements; i++)
		temp[i] = this->set[i];

	for (int i = this->nrOfElements; i < this->capacity; i++)
		temp[i] = NULL;

	delete[] this->set;
	this->set = temp;
}
