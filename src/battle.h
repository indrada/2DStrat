#pragma once
#include "worldMap.h"
#include "person.h"
#include "gui.h"

#include <iostream>

//returns true if player loses
bool initBattle(person* player, person* enemy);

class BattleCore
{
private:

	std::shared_ptr<sf::RectangleShape> playerHero;
	std::shared_ptr<sf::RectangleShape> playerInfoPanel;
	std::shared_ptr<gui::ProgressBar<float>> playerHpBar;
	std::shared_ptr<gui::ProgressBar<float>> playerManaBar;
	std::shared_ptr<gui::ActionsMenu> playerActionMenu;
	std::shared_ptr<gui::ActionsMenu> playerAbilitiesMenu;
	std::shared_ptr<gui::RealTimePrintText> playerLog;

	std::shared_ptr<gui::AbilityInfoPanel> abilityInfoPanel;

	float maxHpRectXSize;

	std::shared_ptr<sf::RectangleShape> enemyHero;
	std::shared_ptr<sf::RectangleShape> enemyInfoPanel;
	std::shared_ptr<gui::ProgressBar<float>> enemyHpBar;
	std::shared_ptr<gui::ProgressBar<float>> enemyManaBar;
	std::shared_ptr<gui::RealTimePrintText> enemyLog;

	std::shared_ptr<sf::Texture> lightTexture;
	std::shared_ptr<sf::Sprite> lightUnderCreature;

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

	bool isPlayerTurn;
	bool inAbilitiesList;

	void initBaseScene();
	void initText();
	void initTextures();
	void initSprites();

	void updateInfoText();
	void updateBuffs();
	
	void playerTurn();
	void enemyTurn();

	void enemyCreatureDeath();
	// create some creature choose mechanincs



public:

	BattleCore(sf::RenderWindow* window, person* person1, person* person2);

	void update();

	void render();

	void handleEvents(sf::Event evt);
};