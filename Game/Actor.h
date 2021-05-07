#pragma once
#include "Entity.h"
#include "Bullet.h"
#include "World.h"
#include "Base.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>



class Actor : public Entity
{
public:
	Actor(const sf::Vector2f &pos, const Base& base, const sf::Color &color);
	~Actor();
	void shoot();
	void shoot(const sf::Vector2f& pos, const sf::Vector2f& velo);
	void drawBullets(sf::RenderWindow &window);
	void clearBullets();
	void checkCollisions(const World& world);
	sf::FloatRect getGlobalBounds() const override;
	const std::list<Bullet*>& getBullets() const;
	std::size_t bulletCount()const { return bullets.size(); };
	const std::list<Bullet*>::iterator removeBullet(const std::list<Bullet*>::const_iterator it);
	sf::Vector2f getBasePos()const;
	sf::FloatRect getBaseBounds()const;
	bool hasDiamond;
	bool killed(const Actor& enemy);


private:
	std::list<Bullet*> bullets; // Bullets that are flying
	const Base base;

};

sf::Packet& operator<<(sf::Packet& packet, Actor& player);
sf::Packet& operator>>(sf::Packet& packet, Actor& player);

bool killed(const Actor& player, const Actor& enemy);
