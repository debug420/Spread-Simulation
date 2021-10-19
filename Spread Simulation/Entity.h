#pragma once
#include <SFML/Graphics.hpp>
#include "Misc.h"

extern sf::VideoMode windowMode;

class Entity
{
private:

	float xVel, yVel;

	void randomizeVelocity()
	{
		int baseVelocity = xVel - 1, maxVelocity = xVel + 1;
		switch (random(1, 8))
		{
		case 1:
		case 2: break;
		case 3: xVel -= 1; break;
		case 4: yVel -= 1; break;
		case 5: xVel += 1; break;
		case 6: yVel += 1; break;
		case 7: xVel += 1; yVel += 1; break;
		case 8: yVel -= 1; xVel -= 1; break;
		}
	}

public:

	std::vector<Entity*> vectorEntityPtr;
	sf::Vertex shape;
	sf::Vector2f pos;
	bool isInfected;
	bool canMove;
	bool dead = false;
	unsigned int infectionAffect;

	// Update function
	void update()
	{

		// Handle infection

		for (int i = 0, n = vectorEntityPtr.size(); i < n; i++)
		{
			if (shape.position == vectorEntityPtr[i]->shape.position)
			{
				if (vectorEntityPtr[i]->isInfected && !isInfected)
				{
					isInfected = true;
					infectionAffect = 20;
				}
				if (isInfected && !vectorEntityPtr[i]->isInfected)
				{
					vectorEntityPtr[i]->isInfected = true;
					vectorEntityPtr[i]->infectionAffect = 20;
				}
			}
		}

		if (infectionAffect <= 0)
			isInfected = false;

		if (isInfected)
			shape.color = sf::Color::Red;
		else
			shape.color = sf::Color::White;

		// Update collisions and velocities
		float borderPadding = windowMode.width / 50;
		if (pos.x >= windowMode.width - borderPadding || pos.x <= borderPadding)
		{
			xVel = -xVel;
			randomizeVelocity();
		}

		if (pos.y >= windowMode.height - borderPadding || pos.y <= borderPadding)
		{
			yVel = -yVel;
			randomizeVelocity();
		}

		// Update movement
		if (canMove)
		{
			pos += sf::Vector2f(xVel, yVel);
		}

		shape.position = pos;
	}

	// Constructor
	Entity(std::vector<Entity*> vectorEntityPtr, unsigned int infectionAffect,
		bool infected = false, bool canMove = true, 
		unsigned int simulationSpeed = 4)
		: vectorEntityPtr(vectorEntityPtr), isInfected(infected), 
		canMove(canMove), xVel(simulationSpeed), yVel(simulationSpeed),
		infectionAffect(infectionAffect)
	{
		shape = sf::Vertex();

		// Randomize starting velocity
		switch (random(1, 4))
		{
		case 1: break;
		case 2: xVel = -xVel; break;
		case 3: yVel = -yVel; break;
		case 4: xVel = -xVel; yVel = -yVel; break;
		}
	}
};

void generateEntities(std::vector<Entity*> &entityList, unsigned int count = 1000, unsigned int numberInfected = 1, unsigned int simulationSpeed = 4, unsigned int infectionAffect = 30)
{
	int generatedInfected = 0;
	const int generationPadding = windowMode.width / 50;

	for (int i = 0; i < count; i++)
	{
		int generatedX = random(generationPadding, windowMode.width - generationPadding);
		int generatedY = random(generationPadding, windowMode.height - generationPadding);
		
		Entity *generatedEntity = new Entity(entityList, infectionAffect, false, true, simulationSpeed);
		if (generatedInfected < numberInfected)
		{
			generatedEntity->isInfected = true;
			generatedInfected++;
		}

		generatedEntity->pos.x = generatedX;
		generatedEntity->pos.y = generatedY;

		generatedEntity->update();

		entityList.push_back(generatedEntity);
	}
}