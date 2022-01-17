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
	sf::Vector3f newRelativePosition = 
		2.0f * (relativePosition - 0.25f * sf::Vector3f(dx>0 ? 1 : -1, dy>0 ? 1 : -1, dz>0 ? 1 : -1));

	int tmpVal = 0;
	//If not zero, step in to, or if scale is one, return
	if(*tree->type != 0 && scale > 1) {
		
		//Gå in i trädet, sedan ska den fortsätta framåt.
		tmpVal =  search(tree, newRelativePosition, scale - 1);
		if (tmpVal != 0) {
			return tmpVal;
		}
	}
	else if (*tree->type != 0)
		return *tree->type;

	//If empty continue forward until wall is hit
	if (/**tree->type == 0*/ true) { //OKLART
		float stepX, stepY, stepZ;
		float tMaxX, tMaxY, tMaxZ;
		float tDeltaX, tDeltaY, tDeltaZ;
		sf::Vector3i blockPosition = sf::Vector3i(dx, dy, dz);
		//Calculare deltas from algotithm:
		//Blocksize can be seen as 1 because the position is relative to current node
		stepX = sgn(direction.x);
		stepY = sgn(direction.y);
		stepZ = sgn(direction.z);

		//Max t delta for x, y and z, TROR ATT + 1.0f är nöfvändigt för att få det rätt [0 ---> 2] istället för [-1 ---> 1]

		//TEST
		newRelativePosition.x -= 0.001f * stepX;
		newRelativePosition.y -= 0.001f * stepY;
		newRelativePosition.z -= 0.001f * stepZ;

		//OBS INTE HELT SÄKER, MEN HAR GJORT EN DEL TEST, MÖJLIGEN RÄTT 
		//T FÅR ALDRIG VARA MINDRE ÄN 0 | FINNS MED STOR SÄKÈRHET FEL HÄR --->:
		tMaxX = (0.5f *stepX - newRelativePosition.x) / direction.x;
		tMaxY = (0.5f *stepY - newRelativePosition.y) / direction.y;
		tMaxZ = (0.5f *stepZ - newRelativePosition.z) / direction.z;
		tMaxX = (direction.x == 0) ? 1000000 : tMaxX;
		tMaxY = (direction.y == 0) ? 1000000 : tMaxY;
		tMaxZ = (direction.z == 0) ? 1000000 : tMaxZ;

		//OM DETTA SKER HAR NÅGOT GÅTT HELT FEL
		if (tMaxX < 0 || tMaxY < 0 || tMaxZ < 0) {
			int h = 0; // DEBUG
		}

		//Calculate delta movement
		tDeltaX = stepX * 1.0f / direction.x;
		tDeltaY = stepY * 1.0f / direction.y;
		tDeltaZ = stepZ * 1.0f / direction.z;
		sf::Vector3f newBlockRelativePosition = sf::Vector3f(0, 0, 0);
		bool hasDoneAtLeastOneSteap = false;
		float minT = 0;
		//kanske kan göras snabbare med någon bit opperation typ.
		while (	blockPosition.x >= 0 && blockPosition.x < 2 &&
				blockPosition.y >= 0 && blockPosition.y < 2 &&
				blockPosition.z >= 0 && blockPosition.z < 2) {
			tree = parent->nodes[blockPosition.x][blockPosition.y][blockPosition.z];
			//If current tree is not empety, search it
			if (*tree->type != 0 && scale > 1 && (hasDoneAtLeastOneSteap || false)) {
				//Hitta den nya relativa positionen. 
				//****      FELET ÄR MED STOR SÄKERHET HÄR: ************ 
				/*
				MÖJLIGA FEL HÄR:
				1) floating point är ligger på linjen o.s.v.
				2) VENNE
				
				
				*/
				
				
				//float minT = (tMaxX < tMaxY) ? ((tMaxX < tMaxZ) ? tMaxX : tMaxZ) : ((tMaxY < tMaxZ) ? tMaxY : tMaxZ);
				sf::Vector3f collitionPoint = direction * (minT) + newRelativePosition;
				sf::Vector3f relPosNewTree =  (collitionPoint - (newBlockRelativePosition));
				int val = search(tree, relPosNewTree, scale - 1);
				if (val != 0)
					return val;
			}
			//TROR DETTA ÄR NÖDVÄNDIGT!
			else if (scale == 1 && *tree->type != 0/*BORDE VARA 1*/) {
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
			hasDoneAtLeastOneSteap = true;
		}
		//Den borde ha nått en vägg. Retunera 0, d.v.s. att den aldrig nådde något.
		return 0;
	}

	std::cout << "BORDE ALDRIG NÅ HIT!\n";
	return 0;
}

//shoot the ray
int TreeRay::shoot(sf::Vector3f relativePosition)
{
	//Start recursive search of the octree:
	//Calculate relative position (relative to (16, 16))
	sf::Vector3f rel = (1.0f/32.0f)* (relativePosition - sf::Vector3f(16.0f, 16.0f, 16.0f));
	return search(root, rel, 5);
}
