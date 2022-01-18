#include "RayCamera.h"
#include <iostream>
#define min(a,b) (a<b?a:b)

sf::Color RayCamera::rayTrace(Ray& ray)
{
	
	return sf::Color::Black;
}

int RayCamera::searchOctree(Ray& ray, int deapth, sf::Vector3f position, Octree* tree)
{
	if (deapth != 0) {
		//If the tree is not empty, search further.
		if (tree->type != 0) {
			
			Ray newRay(ray.getUpdatedPos(), ray.dir);
			ray.step();
			float parentMinT = ray.minT();
			newRay.blockSize = ray.blockSize * 0.5f;
			float newRayTotalT = 0;
			int col = 0;
			//Until the ray is outside of the tree branch repeat:
			while (newRayTotalT*2.0f /* OSÄKER */ < parentMinT-0.0001f)
			{
				newRayTotalT += newRay.minT();
				newRay.step();
				newRay.getFaceValue();
			}
			return col;
		}
		else
		{
			return 0;
		}
	}
	else {
		return *tree->type;
	}
}

RayCamera::RayCamera(sf::Vector2u windowSize)
{
	//Borde bara sparas i octree tror jag.
	cubes = new int** [WORLD_SIZE];
	for (int y = 0; y < WORLD_SIZE; y++) {
		cubes[y] = new int* [WORLD_SIZE];
		for (int z = 0; z < WORLD_SIZE; z++) {
			cubes[y][z] = new int[WORLD_SIZE];
		}
	}
	for (int x = 0; x < WORLD_SIZE; x++) {
		for (int y = 0; y < WORLD_SIZE; y++) {
			for (int z = 0; z < WORLD_SIZE; z++) {
				cubes[x][y][z] = 0;
			}
		}
	}
	cubeTree.instantiate(cubes, 5);
	//cubeTree.fillQuadrant(cubeTree.chunkRoot->nodes[1]);
	//cubeTree.fillQuadrant(cubeTree.chunkRoot->nodes[0][0][0]->nodes[1][1][0]);
	srand(std::time(NULL));

	//SET PALETTE COLORS:
	palette[0] = sf::Color(76 , 86 , 106);
	palette[1] = sf::Color(216, 222, 233);
	palette[2] = sf::Color(191, 97 , 106);
	palette[3] = sf::Color(208, 135, 112);
	palette[4] = sf::Color(235, 203, 139);
	palette[5] = sf::Color(163, 190, 140);
	palette[6] = sf::Color(180, 142, 173);
	palette[7] = sf::Color(136, 192, 208);

	position = sf::Vector3f(5.5f, 3.0f, 25.5f);
	//position = sf::Vector3f(4.0f, 4.0f, 0.5f);
	//cameraDirection = sf::Vector3f(-1, -0.2f, 0);
	cameraDirection = sf::Vector3f(1.0f, 1, -1);
	cameraSize = sf::Vector2i(windowSize.x, windowSize.y);
	image.create(windowSize.x, windowSize.y);
	sprite.setPosition(sf::Vector2f(0, 0));
	sprite.setScale(sf::Vector2f(2.0f, 2.0f)); // 4.0
	imgTexture.create(cameraSize.x, cameraSize.y);
	for (int x = 0; x < WORLD_SIZE; x++)
		for (int y = 0; y < WORLD_SIZE; y++)
			cubes[x][y][0] = 1 + x % 4;
	cubes[4][0][0] = 3;
	//cubes[0][10][0] = 3;
	//Random cubes
	//for (int i = 0; i < 1000; i++) {
	//	int x = rand() % WORLD_SIZE, y = rand() % WORLD_SIZE, z = rand() % WORLD_SIZE;
	//	cubes[x][y][z] = 1 + rand() % 8;
	//}
	for (int x = 0; x < WORLD_SIZE; x++) {
		for (int y = 0; y < WORLD_SIZE; y++) {
			float dist = 16 - std::sqrt(VectorMath::magnitudeSquared(sf::Vector3f(16.0f-x, 16.0f-y, 0)));
			int groundLevel = rand() % 5 + 10 + dist*0.75f;
			for (int z = 0; z < groundLevel && z < WORLD_SIZE; z++) {
				cubes[x][y][z] = groundLevel>13 ? (groundLevel>18 ? 1 : 5) : 4;
			}
		}
	}

	//Set everything to a cube for testing
	//for (int x = 0; x < WORLD_SIZE; x++)
	//	for (int y = 0; y < WORLD_SIZE; y++)
	//		for (int z = 0; z < WORLD_SIZE; z++)
	//			cubes[x][y][z] = 4;

	//Create large sphere
	//sf::Vector3f origin(50, 50, 50);
	//for (int x = 0; x < 100; x++) {
	//	for (int y = 0; y < 100; y++) {
	//		for (int z = 0; z < 100; z++) {
	//			sf::Vector3f deltaPos = sf::Vector3f(x, y, z) - origin;
	//			if(VectorMath::magnitudeSquared(deltaPos) < 150)
	//				cubes[x][y][z] = 1 + rand()%3;
	//		}
	//	}
	//}

	//BUILD THE TREE
	cubeTree.rebuildTree();
}

sf::Sprite RayCamera::renderImage()
{
	time += 0.01f;
	int minReps = 1000, maxReps = 0;
	//std::cout << "Render, time = " << time << "\n";
	sf::Uint8* pixels = new sf::Uint8[((long)cameraSize.x) * ((long)cameraSize.y) * 4l];
	std::memset(pixels, 0, (long)cameraSize.x * (long)cameraSize.y * 4);
	//Camera forward vector
	sf::Vector3f forward = cameraDirection;
	VectorMath::normalize(forward);
	TreeRay ray(position, cubeTree.chunkRoot);

	//Relative vectors
	sf::Vector3f right = VectorMath::corssProduct(sf::Vector3f(0, 0, 1), forward);
	sf::Vector3f up = VectorMath::corssProduct(forward, right);
	for (int y = 0/*360 frame % 1 */; y < cameraSize.y; y += 1) {
		for (int x = 0/*641*/; x < cameraSize.x; x++) {
			//pixel position in world (for ray direction)
			sf::Vector3f rayDirection = forward * 0.5f + right * ((1.0f / (float)cameraSize.x)*((float)x-(cameraSize.x / 2))) + up * ((1.0f / (float)cameraSize.x) * ((float)(cameraSize.y / 2) - (float)y));
			VectorMath::normalize(rayDirection);
			sf::Color col = sf::Color(46, 52, 64);
			float brightness = 255.0f;
			int i = 0;

			//TreeRay ray(cubeTree.chunkRoot, rayDirection);
			int val = ray.shoot(rayDirection/*position*/);
			if (val != 0) {
				col = palette[val];
				int val = ray.getFaceValue();
				brightness = shadowPalette[val];
			}
			i = ray.reps;
			//std::cout << "REaps: " << i << "\n";

			/*TEST*/
			maxReps = (maxReps < i) ? i : maxReps;
			minReps = (i < minReps) ? i : minReps;

			if (!drawComplexity) {
				//Draw the pixel
				int a = 4 * (y * cameraSize.x + x);
				pixels[a + 0] = (sf::Uint8)col.r;
				pixels[a + 1] = (sf::Uint8)col.g;
				pixels[a + 2] = (sf::Uint8)col.b;
				pixels[a + 3] = brightness; //255
			}
			else {
				int a = 4 * (y * cameraSize.x + x);
				float ff = min(i * 2, 255);
				pixels[a + 0] = 255;//(sf::Uint8)col.r;
				pixels[a + 1] = 255;//(sf::Uint8)col.g;
				pixels[a + 2] = 255; (sf::Uint8)col.b;
				pixels[a + 3] = ff; //255
			}
			
		}
	}
	
	
	imgTexture.update(pixels);
	sprite.setTexture(imgTexture);
	delete [] pixels;
	frame++;
	//std::cout << "Min reps = " << minReps << " | max reps = " << maxReps << "\n";
	return sprite;
}

void RayCamera::update(sf::Vector3f movement, sf::Vector3f rotation)
{
	//TEST ADD BLOCKS RANDOMLY
	//int a = rand() % 100;
	//if (a < 50) {
	//	cubeTree.test();
	//	//int x = rand() % 30, y = rand() % 30, z = rand() % 30;
	//	//cubes[x][y][z] = 1 + rand() % 7;
	//}
	//END

	movement *= 0.3f;
	sf::Vector3f right = VectorMath::corssProduct(sf::Vector3f(0, 0, 1), cameraDirection);
	//std::cout << "Movement  = " << movement.x << " " << movement.y << "\n";
	position += cameraDirection * movement.x;
	position += right * movement.y;
	
	rotation *= 0.1f;
	//Rotate around z axies
	sf::Vector3f newRotation = sf::Vector3f(0, 0, 0);
	newRotation.x = cameraDirection.x * std::cos(rotation.z) - cameraDirection.y * std::sin(rotation.z);
	newRotation.y = cameraDirection.x * std::sin(rotation.z) + cameraDirection.y * std::cos(rotation.z);
	newRotation.z = cameraDirection.z + rotation.y;

	cameraDirection = newRotation;
	VectorMath::normalize(cameraDirection);
	//std::cout << "POSITON: " << position.x << " " << position.y << " " << position.z << "\n";
	//std::cout << "ROTATION: " << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << "\n";
}
