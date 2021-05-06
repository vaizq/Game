#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Base : public Entity
{
public:
	Base(const sf::Vector2f& pos, sf::Color color);
	sf::FloatRect getGlobalBounds() const override;
};

