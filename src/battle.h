#pragma once
#include "worldMap.h"
#include "person.h"

#include <iostream>

class BattleWindow
{
private:

	std::shared_ptr<sf::RectangleShape> playerHero;
	std::shared_ptr<sf::RectangleShape> playerInfoPanel;
	std::shared_ptr<sf::RectangleShape> playerHpRect;
	float maxHpRectXSize;

	std::shared_ptr<sf::RectangleShape> enemyHero;
	std::shared_ptr<sf::RectangleShape> enemyInfoPanel;

	sf::Font mainFont;
	std::shared_ptr<sf::Text> playerInfoText;
	std::shared_ptr<sf::Text> enemyInfoText;

	std::shared_ptr<sf::RenderWindow> m_window;
	sf::VideoMode m_vidMode;

	std::vector<std::shared_ptr<sf::RectangleShape>> battleHUD;
	std::vector<std::shared_ptr<sf::Text>> textHUD;

	person* entity1;
	person* entity2;

	void initWindow();
	void initBaseScene();
	void initText();

	void update();
	void updateHp();

	void render();

public:

	BattleWindow(person* person1, person* person2);


};