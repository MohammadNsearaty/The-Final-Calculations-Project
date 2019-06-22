#pragma once
#ifndef OCTREE_H
#define OCTREE_H
#include"BoundingBox.h"
#include<vector>
#include<queue>
#include"Shape.h"
#include<list>
class OcTree {
public:
	//the min/max of the current region
	BoundingBox m_region;
	//the objects in the current region
	std::list<Shapes> m_objects;
	//the objects waiting to be inserted in the tree , we insert them when we want to use them
	//this thing save some cache
	static std::queue<Shapes> m_pendingInsertion;
    // the array of the 8 child octants pointers
	OcTree  *m_childNode[8];
	// this bitmask indicate which child octant is activly being used
	//it saves a lot of comparision operations
	byte m_activeNodes = 0;
	// the min size of the enclosing region is 1x1x1 cube
	const int MIN_SIZE = 1;
	//the life time of the empty region 
	// we will wait maxSpan frames until we delete the current region
	//the maxSapn doubles every time it reUsed until the max is 64
	int maxSpan = 8;
	//the count down of the life span
	int curLife = -1;
	// reference to the parent to help update the tree
	OcTree *parent;
	//check if all objects is inserted in the tree
	static bool ready;
	//there is no tree yet only pendining insertion
	static bool built;

	//the objects in the tree
	std::list<Shapes> allObjects;
public:
	OcTree(BoundingBox region, std::list<Shapes> list)
	{
		this->m_region = region;
		this->m_objects = list;
		this->curLife = -1;
		ready = false;
		built = false;
	}
	OcTree()
	{
		this->m_objects.clear();
		this->m_region = BoundingBox(vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f), vec3(0.0f));
		this->curLife = -1;
	}
	OcTree(BoundingBox region)
	{
		this->m_region = region;
		this->m_objects.clear();
		this->curLife = -1;
	}
	void updateTree();
	void buildTree();
	OcTree CreateNode(BoundingBox region, std::list<Shapes> objList);
	OcTree CreateNode(BoundingBox region, Shapes item);
	void Update(float time);
	void Insert(Shapes s1) {};

};


#endif