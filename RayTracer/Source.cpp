#include <SFML/Graphics.hpp>
#include <iostream>
#include "RayCamera.h"
#include "VectorMath.h"
#include "Ray.h"

int main() {

	//TEST RAYS
	//Ray ray(sf::Vector3f(0, 0, 0), sf::Vector3f(-1.0f, -1.0f, 0.0));
	//for (int i = 0; i < 10; i++) {
	//	std::cout << "Ray position: " << ray.blockPosition.x << " " << ray.blockPosition.y << " " << ray.blockPosition.z << "\n";
	//	ray.step();
	//}
	//int a = 1;
	//std::cin >> a;
	//return 0;
	//END
	//return 0;

	std::cout << "Started" << std::endl;
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "RayCaster", sf::Style::Fullscreen);
	window.setFramerateLimit(60.0f);
	window.setKeyRepeatEnabled(false);
	RayCamera cam(sf::Vector2u(1280, 720));
	sf::Vector3f cross = VectorMath::corssProduct(sf::Vector3f(1, 0, 0), sf::Vector3f(0, 1, 0));
	std::cout << "Cross of a and b is: " << cross.x << " " << cross.y << " " << cross.z << std::endl;
	sf::Vector3f movement = sf::Vector3f(0, 0, 0);
	sf::Vector3f rotation = sf::Vector3f(0, 0, 0);
	sf::Clock clock1;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		}
		clock1.restart();
		window.clear();
		
		movement.x = 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::W) - 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		movement.y = 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::D) - 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		
		rotation.y = 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::Up) - 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		rotation.z = 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - 1.0f * sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

		cam.update(movement , rotation);
		//Draw
		window.draw(cam.renderImage());
		
		window.display();

		auto a = clock1.getElapsedTime();
		std::cout << "FrameTime: " << a.asMilliseconds() << "ms\n";
	}
}