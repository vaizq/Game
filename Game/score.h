#include <SFML/Graphics.hpp>

class Score : public sf::Drawable
{
public:
	int player, enemy;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
