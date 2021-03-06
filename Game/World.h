#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"

enum class Map
{
	Heaven,
	Hell,
	Earth,
	Space,
	Moon,
	Alasateri
};

class World
{
public:
	void build(size_t windowWidth, size_t windowHeight, Map map);
	void addWall(sf::RectangleShape wallRect);
	bool collides(const Entity &e);
	const sf::RectangleShape* collidesWith(const Entity& e) const;
	std::vector<const sf::RectangleShape*> allCollisions(std::vector<const sf::RectangleShape*> &collRects, const Entity& e);
	void draw(sf::RenderWindow &window) const;

private:
	std::vector<sf::RectangleShape> walls;
};


