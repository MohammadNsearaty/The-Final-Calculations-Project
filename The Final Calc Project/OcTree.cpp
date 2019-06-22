#include"OcTree.h"

void OcTree::updateTree()
{
	if (!this->built)
	{
		while (m_pendingInsertion.size() != 0)
		{
			m_objects.push_back(m_pendingInsertion.front());
			m_pendingInsertion.pop();
		}
		buildTree();
	}
	else
	{
		while (m_pendingInsertion.size() != 0)
		{
			Insert(m_pendingInsertion.front());
			m_pendingInsertion.pop();
		}
	}
	ready = true;
}
void OcTree::buildTree()
{
	//check if its a leaf node
	if (m_objects.size() <= 1)
		return;
	//wronnggggg
	vec3 dim = m_region.max - m_region.min;
	if (length(dim) <= 0)//check if dimension is Zero
	{
		//wrong
		findEnclosingCube();
		dim = m_region.max - m_region.min;
	}
	//check if the size of the box is less than the minimum size
	if (dim.x <= MIN_SIZE && dim.y <= MIN_SIZE && dim.z <= MIN_SIZE)
		return;
	vec3 half = dim / 2.0f;
	vec3 center = m_region.min + half;

	BoundingBox octant[8];
	vec3 min = m_region.min;
	vec3 max = m_region.max;
	octant[0] = BoundingBox(min, center);
	octant[1] = BoundingBox(vec3(center.x, min.y, min.z), vec3(max.x,center.y,center.z));
	octant[2] = BoundingBox(vec3(center.x,min.y,center.z), vec3(max.x,center.y,max.z));
	octant[3] = BoundingBox(vec3(min.x,min.y,center.z), vec3(center.x,center.y,max.z));
	octant[4] = BoundingBox(vec3(min.x,center.y,min.z), vec3(center.x,max.y,center.z));
	octant[5] = BoundingBox(vec3(center.x,center.y,min.z), vec3(max.x,max.y,center.z));
	octant[6] = BoundingBox(center,max);
	octant[7] = BoundingBox(vec3(min.x, center.y, center.z), vec3(center.x, max.y, max.z));
	std::list<Shapes> list[8];
	std::list<Shapes> deList;

	for (auto it = m_objects.begin();it !=m_objects.end();it++)
	{
		//shpere
		if (it->getType() == 0)
		{
			for (int j = 0; j < 8; j++)
			{
				if (octant[j].contain(*it))
				{
					list[j].push_back(*it);
					deList.push_back(*it);
					break;
				}
			}

		}
		//cube
		else if (it->getType() == 1)
		{

			for (int j = 0; j < 8; j++)
			{
				if (octant[j].contain(*it))
				{
					list[j].push_back(*it);
					deList.push_back(*it);
					break;
				}
			}
		}
	}
	for (auto k = deList.begin(); k != deList.end();k++)	
	{
		m_objects.remove(*k);
	}
	for (int i = 0; i < 8; i++)
	{
		if (list[i].size() != 0)
		{
			m_childNode[i] = &CreateNode(octant[i], list[i]);
			m_activeNodes |= (byte)(1 << i);
			m_childNode[i]->buildTree();
		}
	}
	built = true;
	ready = true;

}

OcTree OcTree::CreateNode(BoundingBox region, std::list<Shapes> objList)
{
	if (objList.size() == 0)
	{
		OcTree nullTree = OcTree();
		nullTree.parent = nullptr;
		return nullTree;
	}
	OcTree tree = OcTree(region, objList);
	tree.parent = this;
	return tree;
}
OcTree OcTree::CreateNode(BoundingBox region, Shapes item)
{
	std::list<Shapes> list;
	list.push_back(item);
	OcTree tree = OcTree(region, list);
	tree.parent = this;
	return tree;
}
void OcTree::Update(float time)
{
	if (built&&ready)
	{
		if (m_objects.size() == 0)
		{
			if (!m_activeNodes)
			{
				if (curLife == -1)
				{
					curLife = maxSpan;
				}
				else if (curLife > 0)
				{
					curLife--;
				}
			}
		}
		else
		{
			if (curLife != -1)
			{
				if (maxSpan >= 32)
				{
					maxSpan = 64;
				}
				else if (maxSpan < 32)
				{
					maxSpan *= 2;
				}
				curLife = -1;
			}
		}
		//we will try to figure the moving objects
		std::list<Shapes> movedObjects;
		for (auto it = m_objects.begin(); it != m_objects.end(); it++)
		{
			it->Integrate();
			//the integrate method should return boolean and get some duration
			if (it->isAwake())
			{
				movedObjects.push_back(*it);
			}
		}
		//here we must prune any dead objects from the tree
		int listSize = m_objects.size();
		for (auto it = m_objects.begin(); it != m_objects.end(); it++)
		{
			if (!it->Alive())
			{
				for (auto mIt = movedObjects.begin(); mIt != movedObjects.end(); mIt++)
				{
					if (it == mIt)
						movedObjects.remove(*mIt);
					m_objects.remove(*it);
					it--;
				}
				
			}
		}


		//prune out the dead branches in the tree
		for (int an = m_activeNodes, index = 0; an > 0; an >>= 1, index++)
			//check if the node active and the lisfspam is over
			if ((an & 1) == 1 && m_childNode[index]->curLife == 0)
			{
				//if it has some children we keep it alive
				if (m_childNode[index]->m_objects.size() > 0)
				{
					m_childNode[index]->curLife = -1;
				}
				//if it doesn't has children we delete it
				else
				{
					m_childNode[index] = nullptr;
					m_activeNodes ^= (byte)(1 << index);//remove the node from the active node bitmask
				}
			}
		//update the child nodes recursively
		for (int an = m_activeNodes, index = 0; an > 0; an >>= 1, index++)
		{
			if ((an & 1) == 1)
			{
				if (m_childNode != nullptr&& m_childNode[index] != nullptr)
				{
					m_childNode[index]->Update(time);
				}
			}
		}
		//
		for (auto it = movedObjects.begin(); it != movedObjects.end(); it++)
		{
			OcTree *curr = this;
			// we need to compute how up in the tree our moved
			// objects needs to go to reInsert them
			if (!it->getOBB().Empty())
			{
				while (!curr->m_region.contain(*it))
				{
					if (curr->parent != nullptr)
						curr = curr->parent;
					else
					{
						for (auto alIt = allObjects.begin(); alIt != allObjects.end(); alIt++)
						{
							m_pendingInsertion.push(*alIt);
						}
					}
				}
			}
			//we reomve the current moved object from the current node
			// and insert it in the new Containing node
			m_objects.remove(*it);
			curr->Insert(*it);	
		}
		// all the objects are in there correct nodes now we will search for collisions

	}
}