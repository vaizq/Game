#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    void init(sf::VertexArray vertices, sf::Texture texture)
    {
        m_vertices = vertices;
        m_texture = texture;
    }
    virtual sf::FloatRect getGlobalBounds() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_texture;
};

#endif
