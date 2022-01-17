#include "VectorMath.h"

float VectorMath::dot(sf::Vector3f a, sf::Vector3f b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

sf::Vector3f VectorMath::corssProduct(const sf::Vector3f& a, const sf::Vector3f& b)
{
	return sf::Vector3f(a.y*b.z - a.z*b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void VectorMath::normalize(sf::Vector3f& a)
{
	float lengthSqr = a.x * a.x + a.y * a.y + a.z * a.z;
	a = a * (1.0f / std::sqrtf(lengthSqr));
}

float VectorMath::magnitudeSquared(const sf::Vector3f& a)
{
	return a.x*a.x + a.y*a.y + a.z*a.z;
}
