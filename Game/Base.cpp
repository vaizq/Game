#include "Base.h"

Base::Base(const sf::Vector2f& pos, sf::Color color)
{
	sf::VertexArray vertices(sf::Quads, 8);
	vertices[0].position = sf::Vector2f(0, 0);
	vertices[1].position = sf::Vector2f(40, 0);
	vertices[2].position = sf::Vector2f(40, 20);
	vertices[3].position = sf::Vector2f(0, 20);
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;

	vertices[4].position = sf::Vector2f(0, 0);
	vertices[5].position = sf::Vector2f(5, 0);
	vertices[6].position = sf::Vector2f(5, 80);
	vertices[7].position = sf::Vector2f(0, 80);
	vertices[4].color = sf::Color::White;
	vertices[5].color = sf::Color::White;
	vertices[6].color = sf::Color::White;
	vertices[7].color = sf::Color::White;

	sf::Texture tex; // Load texture here
	this->init(vertices, tex);
	this->setOrigin(sf::Vector2f(0, 0));
	this->setPosition(pos);
}

sf::FloatRect Base::getGlobalBounds() const
{
	sf::FloatRect bounds(Entity::getGlobalBounds());
	bounds.height /= 2;
	return bounds;
}
