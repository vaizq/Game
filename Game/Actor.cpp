#include "Actor.h"
#include <cmath>
#include <iostream>

constexpr float PI = 3.14159265f;

Actor::Actor(const sf::Vector2f &pos, const sf::Color &color)
	:hasDiamond(false), bornPos(pos)
{
	sf::VertexArray vertices(sf::Triangles, 3);
	vertices[0].position = sf::Vector2f(0, 0);
	vertices[1].position = sf::Vector2f(20, 20);
	vertices[2].position = sf::Vector2f(0, 40);
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	sf::Texture tex; // Load texture here
	this->init(vertices, tex);
	this->setOrigin(sf::Vector2f(20, 20));
	this->setPosition(pos);
}

Actor::~Actor()
{
	while (!bullets.empty() && bullets.front() != nullptr)
	{
		delete bullets.front();
		bullets.pop_front();
	}
}

/* Add bullet to list and give it velocity and position */
void Actor::shoot()
{
	sf::Vector2f bulletVelo(bulletSpeed * cos((PI / 180) * this->getRotation()), bulletSpeed * sin((PI / 180) * this->getRotation()));
	Bullet* bullet = new Bullet(this->getPosition(), bulletVelo);
	bullets.push_back(bullet);
}

void Actor::shoot(const sf::Vector2f& pos, const sf::Vector2f& velo)
{
	Bullet* bullet = new Bullet(pos, velo);
	bullets.push_back(bullet);
}

/* Draw and move bullets */
void Actor::drawBullets(sf::RenderWindow& window)
{
	for (auto it = bullets.begin(); it != bullets.end(); it++)
	{
		sf::Vector2f bulletPos = (*it)->getPosition();
		if (bulletPos.x < 0 || bulletPos.y < 0 ||
			bulletPos.x >= window.getSize().x || bulletPos.y > window.getSize().y)
		{
			it = bullets.erase(it);
		}
		else
		{
			(*it)->move((*it)->getVelo());
			window.draw(**it);
		}
	}
}

void Actor::clearBullets()
{
	while (!bullets.empty())
	{
		delete bullets.front();
		bullets.pop_front();
	}
}

sf::FloatRect Actor::getGlobalBounds() const
{
	sf::FloatRect bounds(Entity::getGlobalBounds());
	float rotation = this->getRotation();
	if (rotation <= 45 || rotation >= 315)
		bounds.left -= bounds.width;
	else if (rotation <= 315 && rotation >= 225)
	{
		float h = bounds.width;
		bounds.width = bounds.height;
		bounds.height = h;
		bounds.left -= bounds.width / 2;
		bounds.top += bounds.height / 2;
	}
	else if (rotation >= 45 && rotation <= 135)
	{
		float h = bounds.width;
		bounds.width = bounds.height;
		bounds.height = h;
		bounds.left -= bounds.width / 2;
		bounds.top -= bounds.height / 2;
	}

	bounds.top -= bounds.height / 2;
	return bounds;
}

const std::list<Bullet*>& Actor::getBullets() const
{
	return bullets;
}

const std::list<Bullet*>::iterator Actor::removeBullet(const std::list<Bullet*>::const_iterator it)
{
	return bullets.erase(it);
}

bool Actor::killed(const Actor& enemy)
{
	const sf::FloatRect& enemyBounds(enemy.getGlobalBounds());
	for (const auto& bullet : getBullets())
	{
		if (bullet->getGlobalBounds().intersects(enemyBounds))
			return true;
	}
	return false;
}


sf::Packet& operator<<(sf::Packet& packet, Actor& player)
{
	packet << player.getPosition().x << player.getPosition().y;
	packet << player.bulletCount();
	for (auto bullet : player.getBullets())
		packet << bullet->getPosition().x << bullet->getPosition().y << bullet->getVelo().x << bullet->getVelo().y;

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Actor& player)
{
	sf::Vector2f newPos;
	packet >> newPos.x >> newPos.y;
	player.setPosition(newPos);
	size_t bulletCount;
	packet >> bulletCount;

	player.clearBullets();
	sf::Vector2f pos, velo;
	for (int i = 0; i < bulletCount; i++)
	{
		packet >> pos.x >> pos.y >> velo.x >> velo.y;
		player.shoot(pos, velo);// 'Shoots': bullet starts moving from pos and moves with velo
	}
	return packet;
}

bool killed(const Actor& player, const Actor& enemy)
{
	const sf::FloatRect& enemyBounds(enemy.getGlobalBounds());
	for (const auto& bullet : player.getBullets())
	{
		if (bullet->getGlobalBounds().intersects(enemyBounds))
			return true;
	}
	return false;
}





