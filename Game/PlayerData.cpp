#include "PlayerData.h"

PlayerData::PlayerData(const Actor& player)
	:pos{player.getPosition()}
{
	std::list<Bullet*> bullets{ player.getBullets() };
}

