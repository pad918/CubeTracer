#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Definitions.h"
#include "VectorMath.h"

class  Ray
{
public:
	sf::Vector3f pos, dir;
	Ray(sf::Vector3f pos, sf::Vector3f dir);
	sf::Vector3i blockPosition;
	float blockSize = 1.0f;
	inline void step() {
		//Branched
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				lastBlockFace = stepX;
				blockPosition.x += stepX;
				tMaxX = tMaxX + tDeltaX;
			}
			else {
				lastBlockFace = 3 * stepZ;
				blockPosition.z += stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
		else {
			if (tMaxY < tMaxZ) {
				lastBlockFace = 2 * stepY;
				blockPosition.y += stepY;
				tMaxY = tMaxY + tDeltaY;
			}
			else {
				lastBlockFace = 3 * stepZ;
				blockPosition.z += stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		}
	}

	inline bool isInBounds() {
		return blockPosition.x >= 0 && blockPosition.x < WORLD_SIZE && blockPosition.y >= 0 &&
			blockPosition.y < WORLD_SIZE && blockPosition.z >= 0 && blockPosition.z < WORLD_SIZE;
	}

	inline int getFaceValue() {
		return lastBlockFace + 3 - (lastBlockFace>0 ? 1 : 0);
	};
	inline sf::Vector3f getUpdatedPos(){
		float minT = (tMaxX < tMaxY) ? ((tMaxX < tMaxZ) ? tMaxX : tMaxZ) : ((tMaxY < tMaxZ) ? tMaxY : tMaxZ);
		return pos + minT * dir;
	}
	inline float minT() {
		return (tMaxX < tMaxY) ? ((tMaxX < tMaxZ) ? tDeltaX : tDeltaZ) : ((tMaxY < tMaxZ) ? tDeltaY : tDeltaZ);
	}
private:
	
	// http://www.cse.yorku.ca/~amana/research/grid.pdf implementation:
	float stepX, stepY, stepZ;
	float tMaxX, tMaxY, tMaxZ;
	float tDeltaX, tDeltaY, tDeltaZ;
	int lastBlockFace = 0;
	sf::Vector3f direction;
	sf::Vector3f offset;
};



