#include "Plane.h"

bool Plane::willHit(const Ray& ray)
{
	//Calculate hitposition
	float t = -1.0f; //  *(VectorMath::dot(normal, ray.position)) / (VectorMath::dot(normal, ray.direction));
	return t > 0;
}

float Plane::hitDistance(const Ray& ray)
{
	float t = -1.0f; // *(VectorMath::dot(normal, ray.position)) / (VectorMath::dot(normal, ray.direction));
	return t;
}

sf::Vector3f& Plane::getNormal(const sf::Vector3f& pos)
{
	return normal;
}

Plane::Plane()
{
	normal = sf::Vector3f(0, 0, 1); // Point up
	surfaceColor = sf::Color(51, 200, 51);
}
