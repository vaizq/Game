#include "Entity.h"
#include <cmath>

sf::FloatRect Entity::getGlobalBounds() const
{
    sf::FloatRect bounds = m_vertices.getBounds();
    bounds.left = this->getPosition().x;
    bounds.top = this->getPosition().y;
    return bounds;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // apply the texture
    states.texture = &m_texture;

    // you may also override states.shader or states.blendMode if you want

    // draw the vertex array
    target.draw(m_vertices, states);
}