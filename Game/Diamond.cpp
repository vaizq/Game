#include "Diamond.h"

Diamond::Diamond(sf::Vector2f pos, sf::Color color)
{
	sf::VertexArray vertices(sf::Quads, 4);
	vertices[0].position = sf::Vector2f(20, 0);
	vertices[1].position = sf::Vector2f(0, 20);
	vertices[2].position = sf::Vector2f(20, 40);
	vertices[3].position = sf::Vector2f(40, 20);
	vertices[0].color = sf::Color::Red;
	vertices[1].color = sf::Color::Blue;
	vertices[2].color = sf::Color::Green;
	vertices[3].color = sf::Color::Yellow;
	sf::Texture tex; // Load texture here
	this->init(vertices, tex);
	this->setOrigin(sf::Vector2f(20, 20));
	this->setPosition(pos);
}

sf::FloatRect Diamond::getGlobalBounds() const
{
	sf::FloatRect defaultBounds(Entity::getGlobalBounds());
	sf::FloatRect bounds(sf::Vector2f(defaultBounds.left - defaultBounds.width / 2, defaultBounds.top - defaultBounds.height / 2),
					     sf::Vector2f(defaultBounds.width, defaultBounds.height));
	return bounds;
}
