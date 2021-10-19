#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>

#include "Entity.h"

sf::VideoMode windowMode;

int main()
{
	unsigned int INFECTED = 1;
	unsigned int ENTITYCOUNT = 1000;
	unsigned int SIMULATIONSPEED = 4;
	unsigned int INFECTIONAFFECT = 30;
	windowMode.height = 500;
	windowMode.width = 500;

	{
		char settingModifyAnswer;
		std::cout << "Modify settings? (y/n): "; std::cin >> settingModifyAnswer;
		if (settingModifyAnswer != 'y')
			goto startProgram;

		std::cout << "Infected (RECOMMENDED: 1): "; std::cin >> INFECTED;
		std::cout << "Entity Count (RECOMMENDED: 1000): "; std::cin >> ENTITYCOUNT;
		std::cout << "Simulation Speed (RECOMMENDED: 4): "; std::cin >> SIMULATIONSPEED;
		std::cout << "Infection Time (How long it will take to be cured/die in seconds) (RECOMMENDED: 30): "; std::cin >> INFECTIONAFFECT;
		std::cout << "Window Size X (RECOMMENDED: 500): "; std::cin >> windowMode.width;
		std::cout << "Window Size Y (RECOMMENDED 500): "; std::cin >> windowMode.height;
	}

	startProgram:

	std::vector<Entity*> entityList;
	unsigned int infectedCount;
	unsigned int deadCount;

	sf::RenderWindow window;
	window.create(windowMode, "Spread Simulation");
	window.setFramerateLimit(30);

	generateEntities(entityList, ENTITYCOUNT, INFECTED, SIMULATIONSPEED, INFECTIONAFFECT);
	std::vector<sf::Vertex> renderArray;
	renderArray.resize(ENTITYCOUNT);

	sf::Clock timer;
	while (window.isOpen())
	{

		// Handle events
		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Update all instances
		for (Entity* v : entityList)
			v->update();

		if (timer.getElapsedTime().asMilliseconds() >= 1000)
		{
			for (Entity* v : entityList)
			{
				if (v->infectionAffect > 0)
					v->infectionAffect--;
				if (v->isInfected && random(1, 15) == 1)
				{
					v->dead = true;
				}
			}
			timer.restart();
		}

		// Rendering
		window.clear(sf::Color::Black);

		renderArray.clear();

		infectedCount = 0;
		deadCount = 0;
		for (Entity* v : entityList)
		{
			if (v->isInfected && !(v->dead))
				infectedCount++;
			if (v->dead)
				deadCount++;
			if (!(v->dead))
				renderArray.insert(renderArray.begin(), v->shape);
		}
		std::cout << "Infected : " << infectedCount << std::endl;
		std::cout << "Dead : " << deadCount << std::endl;

		window.draw(&renderArray[0], renderArray.size(), sf::Points);
		window.display();

	}
	
	for (Entity* entity : entityList)
	{
		delete entity;
	}

	return 0;
}