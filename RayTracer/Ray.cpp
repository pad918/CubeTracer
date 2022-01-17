#include "Ray.h"
#include <iostream>
#define sgn(x) (x>=0 ? 1 : -1)

Ray::Ray(sf::Vector3f pos, sf::Vector3f dir)
{
	this->pos = pos;
	this->dir = dir;
	pos *= (1.0f / blockSize);
	blockPosition = (sf::Vector3i) ((pos) + sf::Vector3f(0.5f, 0.5f, 0.5f)); // bättre avrundning

	//PRE CALCUALTIONS

	//step sizes
	stepX = sgn(dir.x);
	stepY = sgn(dir.y);
	stepZ = sgn(dir.z);

	//Max t delta for x, y and z
	tMaxX = (stepX * (blockSize*0.5f) - blockSize * (pos.x - (float) blockPosition.x)) / dir.x;
	tMaxY = (stepY * (blockSize*0.5f) - blockSize * (pos.y - (float) blockPosition.y)) / dir.y;
	tMaxZ = (stepZ * (blockSize*0.5f) - blockSize * (pos.z - (float) blockPosition.z)) / dir.z;

	//Calculate delta movement
	tDeltaX = sgn(dir.x) * blockSize / dir.x;
	tDeltaY = sgn(dir.y) * blockSize / dir.y;
	tDeltaZ = sgn(dir.z) * blockSize / dir.z;
}

