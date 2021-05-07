#include "World.h"

void World::build(size_t windowWidth, size_t windowHeight, Map map)
{
	std::srand((int)map);
    for (int x = 0; x < windowWidth; x += 100)
    {
        for (int y = 0; y < windowHeight; y += 100)
        {
            if (std::rand() % 5 == 0 && x + y != 0 && ( x < windowWidth - 100 && y < windowWidth - 100))
            {
                sf::RectangleShape rect(sf::Vector2f(100, 100));
                rect.setFillColor(sf::Color(x % 255, y % 255, 255, 255));
                rect.move(x, y);
                this->addWall(rect);
            }
        }

    }
    /* Rects to make window bounds */
    float dp = (float)windowWidth;
    sf::RectangleShape left(sf::Vector2f(windowWidth, windowHeight));
    left.move(-dp, 0);
    this->addWall(left);
    
    sf::RectangleShape right(sf::Vector2f(windowWidth, windowHeight));
    right.move(dp, 0);
    this->addWall(right);  
    
    dp = windowHeight;
    sf::RectangleShape up(sf::Vector2f(windowWidth, windowHeight));
    up.move(0, dp);
    this->addWall(up);
    
    dp = windowHeight;
    sf::RectangleShape down(sf::Vector2f(windowWidth, windowHeight));
    down.move(0, -dp);
    this->addWall(down);
}

void World::addWall(sf::RectangleShape wallRect)
{
    walls.push_back(wallRect);
}

bool World::collides(const Entity &e)
{
    sf::FloatRect entityBounds = e.getGlobalBounds();
    for (auto wall : walls)
    {
        if (wall.getGlobalBounds().intersects(entityBounds))
            return true;
    }
    return false;
}

/* Returns wall(Rectangle shape) of witch entity collides with 
   nullptr if no collisions.                                     
*/
const sf::RectangleShape* World::collidesWith(const Entity& e)const
{
    sf::FloatRect entityBounds = e.getGlobalBounds();
    for (const auto &wall : walls)
    {
        if (wall.getGlobalBounds().intersects(entityBounds))
            return &wall;
    }
    return nullptr;
}

std::vector<const sf::RectangleShape*> World::allCollisions(std::vector<const sf::RectangleShape*> &collRects, const Entity& e)
{
    sf::FloatRect entityBounds = e.getGlobalBounds();
    for (const auto &wall : walls)
    {
        if (wall.getGlobalBounds().intersects(entityBounds))
            collRects.push_back(&wall);
    }
    return collRects;
}

void World::draw(sf::RenderWindow &window) const
{
    for (auto wall : walls)
        window.draw(wall);
}

