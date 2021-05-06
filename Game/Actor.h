#pragma once
#include "Entity.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <list>



class Actor : public Entity
{
public:
	Actor(const sf::Vector2f &pos, const sf::Color &color);
	~Actor();
	void shoot();
	void drawBullets(sf::RenderWindow &window);
	sf::FloatRect getGlobalBounds() const override;
	const std::list<Bullet*>& getBullets();
	const std::list<Bullet*>::iterator removeBullet(const std::list<Bullet*>::const_iterator it);
	bool hasDiamond;
	bool isShot;
private:
	std::list<Bullet*> bullets; // Bullets that are flying
};

