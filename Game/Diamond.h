#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Diamond : public Entity
{
public:
	Diamond(sf::Vector2f pos, sf::Color color);
	sf::FloatRect getGlobalBounds() const override;
	
};

