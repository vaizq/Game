#include "Bullet.h"
#include <cmath>

constexpr float PI = 3.14159265359;


Bullet::Bullet(const sf::Vector2f& pos, const sf::Vector2f& velo, const sf::Color &color)
	:m_velo(velo)
{
	sf::VertexArray vertices(sf::TriangleFan, 12);
	constexpr float step = 2 * PI / 10;
	constexpr float r = 5;
	float angle = 0;
	vertices[0].position = sf::Vector2f(0, 0);
	for (int i = 1; i < 11; i++, angle += step)
	{
		vertices[i].position = sf::Vector2f(r * cos(angle),  r * sin(angle));
		vertices[i].color = color;
	}
	vertices[11].position = sf::Vector2f(r, 0);

	sf::Texture tex; // Load texture here
	this->init(vertices, tex);
	this->setOrigin(sf::Vector2f(0, 0));
	this->setPosition(pos);
}

