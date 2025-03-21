#pragma once
#include "worldMap.h"
#include "person.h"

#include <iostream>

class BattleCore
{
private:

	std::shared_ptr<sf::RectangleShape> playerHero;
	std::shared_ptr<sf::RectangleShape> playerInfoPanel;
	std::shared_ptr<sf::RectangleShape> playerHpRect;
	float maxHpRectXSize;

	std::shared_ptr<sf::RectangleShape> enemyHero;
	std::shared_ptr<sf::RectangleShape> enemyInfoPanel;

	sf::RenderWindow* m_window;

	sf::Font mainFont;
	std::shared_ptr<sf::Text> playerInfoText;
	std::shared_ptr<sf::Text> enemyInfoText;

	std::vector<std::shared_ptr<sf::RectangleShape>> battleHUD;
	std::vector<std::shared_ptr<sf::Text>> textHUD;

	person* entity1;
	person* entity2;

	std::shared_ptr<Creature> friendlyCreature;
	std::shared_ptr<Creature> enemyCreature;

	bool playerTurn;

	void initBaseScene();
	void initText();

	void updateInfoText();

	// create some creature choose mechanincs



public:

	BattleCore(sf::RenderWindow* window, person* person1, person* person2);

	void update();

	void render();

	void handleEvents(sf::Event evt);
};