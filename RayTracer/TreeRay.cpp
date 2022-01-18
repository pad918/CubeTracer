#include "ChunkTree.h"
#include "TreeRay.h"
#include <iostream>
#define sgn(x) (x>=0 ? 1 : -1)


TreeRay::TreeRay(Octree* rootTree,  sf::Vector3f direction)
{
	this->direction = direction;
	root = rootTree;
}

//Relative position is in the parets cordinate system between -0.5 and 0.5;
int TreeRay::search(Octree* parent, sf::Vector3f relativePosition, int scale)
{
	reps++;
	int dx = (relativePosition.x >= 0) ? 1 : 0;
	int dy = (relativePosition.y >= 0) ? 1 : 0;
	int dz = (relativePosition.z >= 0) ? 1 : 0;

	//next tree subdivition:
	Octree* tree = parent->nodes[dx][dy][dz];
	
	if (scale == 0 && *tree->type != 0)
		return *tree->type;

	{
		//Calculate the position relative to the new tree node
		sf::Vector3f newRelativePosition =
			2.0f * (relativePosition - 0.25f * sf::Vector3f(dx > 0 ? 1 : -1, dy > 0 ? 1 : -1, dz > 0 ? 1 : -1));

		sf::Vector3i blockPosition = sf::Vector3i(dx, dy, dz);

		float tMaxX, tMaxY, tMaxZ;
		tMaxX = (0.5f *stepX - newRelativePosition.x) / direction.x;
		tMaxY = (0.5f *stepY - newRelativePosition.y) / direction.y;
		tMaxZ = (0.5f *stepZ - newRelativePosition.z) / direction.z;

		sf::Vector3f newBlockRelativePosition = sf::Vector3f(0, 0, 0);
		float minT = 0;

		while ((((uint32_t)blockPosition.x | (uint32_t)blockPosition.y | (uint32_t)blockPosition.z) & 0xFFFFFFFE) == 0) {

			tree = parent->nodes[blockPosition.x][blockPosition.y][blockPosition.z];
			//If current tree is not empety, search it
			if (*tree->type != 0 && scale > 1) {
				
				sf::Vector3f collitionPoint = direction * (minT) + newRelativePosition;
				sf::Vector3f relPosNewTree =  (collitionPoint - (newBlockRelativePosition));
				int val = search(tree, relPosNewTree, scale - 1);
				if (val != 0)
					return val;
			}
			//SKRIV EN KOMMENTAR HÄR
			else if (scale == 1 && *tree->type != 0) {
				return *tree->type;
			}
			if (tMaxX < tMaxY) {
				if (tMaxX < tMaxZ) {
					lastBlockFace = stepX;
					blockPosition.x += stepX;
					minT = tMaxX;
					tMaxX = tMaxX + tDeltaX;
					newBlockRelativePosition.x += stepX;
				}
				else {
					lastBlockFace = stepZ * 3;
					blockPosition.z += stepZ;
					minT = tMaxZ;
					tMaxZ = tMaxZ + tDeltaZ;
					newBlockRelativePosition.z += stepZ;
				}
			}
			else {
				if (tMaxY < tMaxZ) {
					lastBlockFace = stepY * 2;
					blockPosition.y += stepY;
					minT = tMaxY;
					tMaxY = tMaxY + tDeltaY;
					newBlockRelativePosition.y += stepY;
				}
				else {
					lastBlockFace = stepZ * 3;
					blockPosition.z += stepZ;
					minT = tMaxZ;
					tMaxZ = tMaxZ + tDeltaZ;
					newBlockRelativePosition.z += stepZ;
				}
			}
		}
		//Den borde ha nått en vägg. Retunera 0, d.v.s. att den aldrig nådde något.
		return 0;
	}
	throw std::domain_error("FIX THIS MESS ERROR");
	return 0;
}

//shoot the ray
int TreeRay::shoot(sf::Vector3f relativePosition)
{
	//Pre calculations
	stepX = sgn(direction.x);
	stepY = sgn(direction.y);
	stepZ = sgn(direction.z);

	//Calculate delta movement
	tDeltaX = stepX * 1.0f / direction.x;
	tDeltaY = stepY * 1.0f / direction.y;
	tDeltaZ = stepZ * 1.0f / direction.z;

	//Start recursive search of the octree:
	//Calculate relative position (relative to (16, 16))
	sf::Vector3f rel = (1.0f/32.0f)* (relativePosition - sf::Vector3f(16.0f, 16.0f, 16.0f));
	return search(root, rel, 5);
}
