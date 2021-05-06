#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Entity.h"
#include "World.h"
#include "Actor.h"
#include "Bullet.h"
#include "Diamond.h"
#include "Base.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720
constexpr float PI = (float)3.14159265;
#define SPEED 0.5

constexpr bool SERVER = false; 
constexpr int PORT = 53000;
constexpr const char* IP_ADDRESS = "192.168.43.128";

int main()
{
	World world;
	world.build(WINDOW_WIDTH, WINDOW_HEIGHT);

	/* Initialize hero and enemy depending is this 'server' or 'client' machine*/
	sf::Vector2f heroBornPos;
	sf::Vector2f enemyBornPos;
	sf::Color heroColor;
	sf::Color enemyColor;

	if (SERVER)
	{
		heroBornPos = sf::Vector2f(20, 20);
		heroColor = sf::Color::Blue;
		enemyBornPos = sf::Vector2f(WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20);
		enemyColor = sf::Color::Green;
	}
	else
	{
		heroBornPos = sf::Vector2f(WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20);
		heroColor = sf::Color::Green;
		enemyBornPos = 	sf::Vector2f(20, 20);
		enemyColor = sf::Color::Blue;
	}
	Actor hero(heroBornPos, heroColor);
	Actor enemy(enemyBornPos, enemyColor);

	Diamond diamond(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Color::Blue);
	sf::Vector2f pBasePos;
	sf::Vector2f eBasePos;
	if (SERVER)
	{
		pBasePos = sf::Vector2f(10, 10);
		eBasePos = sf::Vector2f(WINDOW_WIDTH - 40, WINDOW_HEIGHT - 90);
	}
	else
	{
		pBasePos = sf::Vector2f(WINDOW_WIDTH - 40, WINDOW_HEIGHT - 90);
		eBasePos = sf::Vector2f(10, 10);
	}

	Base playerBase(pBasePos, sf::Color::Red);
	Base enemyBase(eBasePos, sf::Color::Red);
	
	/* Connect with client and server */
	sf::TcpListener listener;
	if (SERVER)
	{
		if (listener.listen(PORT) != sf::Socket::Done)
		{
			std::cout << "Server failed to listen" << std::endl;
		}
	}
	sf::TcpSocket socket;

	if (SERVER)
	{
		std::cout << "Waiting for connections..." << std::endl;
		if (listener.accept(socket) != sf::Socket::Done)
		{
			std::cout << "Failed to accept connection" << std::endl;
		}
		else
		{
			std::cout << "Connected" << std::endl;
		}
	}
	else
	{
		std::cout << "Connecting..." << std::endl;
		if (socket.connect(IP_ADDRESS, PORT) != sf::Socket::Done)
		{
			std::cout << "Failed to connecto to " << IP_ADDRESS << " Port " << PORT << std::endl;
		}
		else
		{
			std::cout << "Connected succesfully" << std::endl;
		}
	}
	socket.setBlocking(false);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shoot em all");

	while (window.isOpen())
	{
		/* Check if hero killed enemy */
		const std::list<Bullet*> bulletList{ hero.getBullets() };
		bool enemyGotShot;
		sf::FloatRect enemyBounds{ enemy.getGlobalBounds() };
		for (auto bullet : bulletList)
		{
			if (bullet->getGlobalBounds().intersects(enemyBounds))
			{
				enemyGotShot = true;
			}
		}
		/* Get enemy position and send hero position out */
		sf::Vector2f enemyPos = enemy.getPosition();
		sf::Packet heroData;
		sf::Packet enemyData;
		heroData << hero.getPosition().x << hero.getPosition().y << enemyGotShot;
		if (SERVER)
		{
			if (socket.receive(enemyData) == sf::Socket::Done)
			{
				enemyData >> enemyPos.x >> enemyPos.y >> hero.isShot;
				enemy.setPosition(enemyPos);
				if (hero.isShot)
				{
					hero.setPosition(heroBornPos);
					hero.isShot = false;
				}
			}

			if (socket.send(heroData) != sf::Socket::Done)
			{
				//std::cout << "Client failed to receive data" << std::endl;
			}
		}
		else
		{
			if (socket.send(heroData) != sf::Socket::Done)
			{
				//std::cout << "Server failed to receive data" << std::endl;
			}

			if (socket.receive(enemyData) == sf::Socket::Done)
			{
				enemyData >> enemyPos.x >> enemyPos.y >> hero.isShot;
				enemy.setPosition(enemyPos);
				if (hero.isShot)
				{
					hero.setPosition(heroBornPos);
					hero.isShot = false;
				}
			}
		}

		/* Handle events and move hero accordingly */
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					hero.shoot();
				}
			}
		}

		/* Handle movement */
		sf::Vector2f movement(0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			movement.y = -1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			movement.y = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			movement.x = -1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			movement.x = 1;
		}
		float movementLen = (float)sqrt(pow(movement.x, 2) + pow(movement.y, 2));
		if (movementLen != 0)
		{
			movement.x = (float)SPEED * movement.x / movementLen;
			movement.y = (float)SPEED * movement.y / movementLen;
			hero.move(movement);
		}

		/* Handle mouse and shooting */
		sf::Vector2i cursorPos = sf::Mouse::getPosition(window);
		sf::Vector2f heroPos = hero.getPosition();
		float angle = 0;
		if (cursorPos.x > heroPos.x)
		{
			angle = (float)atan((cursorPos.y - heroPos.y) / (cursorPos.x - heroPos.x));
		}
		else if (cursorPos.x == heroPos.x)
		{
			if (cursorPos.y > heroPos.y)
				angle = PI;
			else
				angle = 0;
		}
		else
		{
			angle = PI + (float)atan((cursorPos.y - heroPos.y) / (cursorPos.x - heroPos.x));
		}
		hero.setRotation((180 / PI) * angle);

		/* Handle collisions with world and hero */
		std::vector<const sf::RectangleShape*> collideRects;
		world.allCollisions(collideRects, hero);
		for(auto collideRec : collideRects)
		{
			// Handle collision
			sf::Vector2f bounceDir{ hero.getPosition().x - (collideRec->getPosition().x + collideRec->getSize().x/2),
								    hero.getPosition().y - (collideRec->getPosition().y + collideRec->getSize().y/2) };

			sf::Vector2f moveVec(0, 0);
			// Check in witch of the four sides hero is colliding
			constexpr float moveOutSpeed = 2 * 2 * SPEED;
			if (abs(bounceDir.x)/collideRec->getSize().x > abs(bounceDir.y)/collideRec->getSize().y)
			{			
				moveVec.x = moveOutSpeed * bounceDir.x / abs(bounceDir.x);
			}
			else
			{
				moveVec.y = moveOutSpeed * bounceDir.y / abs(bounceDir.y);
			}
			
			hero.move(moveVec);
		}

		/* Handle collisions with world and bullets */
		const std::list<Bullet*> &bullets = hero.getBullets();
		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			if (world.collidesWith(**it))
			{
				it = hero.removeBullet(it);
			}
		}

		if (hero.getGlobalBounds().intersects(diamond.getGlobalBounds()) && 
			!enemy.hasDiamond)
			hero.hasDiamond = true;
		if (hero.getGlobalBounds().intersects(playerBase.getGlobalBounds()))
			hero.hasDiamond = false;
		if (enemy.getGlobalBounds().intersects(diamond.getGlobalBounds()) &&
			!hero.hasDiamond)
			enemy.hasDiamond = true;
		if (enemy.getGlobalBounds().intersects(enemyBase.getGlobalBounds()))
			enemy.hasDiamond = false;

		window.clear();
		world.draw(window);
		window.draw(enemyBase);
		window.draw(playerBase);
		if(!hero.hasDiamond && !enemy.hasDiamond)
			window.draw(diamond);
		window.draw(enemy);
		window.draw(hero);
		hero.drawBullets(window);
		window.display();
	}

	return 0;
}