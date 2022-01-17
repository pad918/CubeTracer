#include "Sphere.h"

Sphere::Sphere(float r)
{
	position = sf::Vector3f(50, 0, 1);
	surfaceColor = sf::Color(200, 51, 51);
	radius = r;
}

bool Sphere::willHit(const Ray& ray)
{
	//sf::Vector3f L = position - ray.position;
	//float t = VectorMath::dot(L, ray.direction);
	//return t > 0;
	return false;
}

float Sphere::hitDistance(const Ray& ray)
{
	///*Stolen mathfs https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection */
	//sf::Vector3f L = position - ray.position;
	//float tca = VectorMath::dot(L, ray.direction); //Projection on unitvector is simple. if(tca < 0) no positive intersection
	//
	////Early quit if not in positive ray direction
	//if(tca < 0)
	//	return -1;
	//
	//float dSqr = VectorMath::dot(L, L) - tca * tca;
	//float thc = std::sqrt(radius*radius - dSqr);
	//float t = thc > 0 ? tca - thc : tca + thc;
	//return t;
	return false;
}

sf::Vector3f& Sphere::getNormal(const sf::Vector3f& pos)
{
	sf::Vector3f normal = pos - position;
	VectorMath::normalize(normal);
	return normal;
}
