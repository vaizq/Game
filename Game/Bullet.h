#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

constexpr float bulletSpeed = 3;

class Bullet : public Entity
{
public:
	Bullet(const sf::Vector2f& pos, const sf::Vector2f& velo, const sf::Color &color = sf::Color::Red);

	sf::Vector2f getVelo() { return m_velo; }
private:
	sf::Vector2f m_velo;
};

