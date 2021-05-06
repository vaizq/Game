#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Actor.h"
#include "Bullet.h"
#include <list>
#include <vector>

class PlayerData
{
public:
	PlayerData(const Actor& player);
	
private:
	sf::Vector2f pos;
	std::vector<Bullet> bullets;
};

sf::Packet operator<<(sf::Packet packet, PlayerData data);
sf::Packet operator>>(sf::Packet packet, PlayerData data);

