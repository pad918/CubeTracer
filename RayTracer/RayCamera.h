#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <vector>
#include "Definitions.h"
#include "Ray.h"
#include "ChunkTree.h"
#include "TreeRay.h"

/*
Benchmarks: (Kortaste frametimes under ca 100 frames)
1.
	Release: 187ms
	Debug:   1667ms

2.
	Release: 181ms
	
3.
	Release: 174ms

4.
	Release: 162ms



*/

class RayCamera {
private:
	//int cubes[WORLD_SIZE][WORLD_SIZE][WORLD_SIZE] = {};
	//std::vector<std::vector<std::vector<int>>> cubes;
	int*** cubes;
	ChunkTree cubeTree;
	//std::vector <Cube> cubes; //should be smartPointer
	sf::Color palette[8];
	float shadowPalette[6] = {255, 150, 150, 200, 200, 100};
	sf::Image image;
	sf::Texture imgTexture;
	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f cameraDirection;
	sf::Color rayTrace(Ray & ray);
	int searchOctree(Ray& ray, int deapth, sf::Vector3f position, Octree * tree);
	float time = 0;
	int frame = 0;
	bool drawComplexity = false;
	
public:
	RayCamera(sf::Vector2u windowSize);
	sf::Sprite renderImage();
	void update(sf::Vector3f movement, sf::Vector3f rotation);
	sf::Vector2i cameraSize;
};