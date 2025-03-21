#include "battle.h"

void BattleCore::initBaseScene()
{
	
	//change it to player and enemy sprites then
	playerHero = std::make_shared<sf::RectangleShape>();
	playerHero->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 200.f , (m_window->getSize().y / 1080.f) * 100.f });
	playerHero->setFillColor(sf::Color::Green);
	playerHero->setPosition(sf::Vector2f{ m_window->getSize().x * 0.2f, m_window->getSize().y * 0.8f});

	enemyHero = std::make_shared<sf::RectangleShape>();
	enemyHero->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 200.f , (m_window->getSize().y / 1080.f) * 100.f });
	enemyHero->setFillColor(sf::Color::Red);
	enemyHero->setPosition(sf::Vector2f{ m_window->getSize().x * 0.8f, m_window->getSize().y * 0.2f - enemyHero->getGlobalBounds().size.y });

	battleHUD.push_back(playerHero);
	battleHUD.push_back(enemyHero);

	
	//player control panel
	playerInfoPanel = std::make_shared<sf::RectangleShape>();
	playerInfoPanel->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 700.f , (m_window->getSize().y / 1080.f) * 300.f });
	playerInfoPanel->setFillColor(sf::Color{ 128,128,128,50 });
	playerInfoPanel->setPosition(sf::Vector2f{ m_window->getSize().x * 0.6f, m_window->getSize().y * 0.6f });

	playerHpRect = std::make_shared<sf::RectangleShape>();
	playerHpRect->setSize(sf::Vector2f{ playerInfoPanel->getSize().x, 15.f });
	playerHpRect->setPosition(sf::Vector2f{ playerInfoPanel->getPosition().x, playerInfoPanel->getPosition().y + playerInfoPanel->getPosition().y * 0.2f });
	playerHpRect->setFillColor(sf::Color::Red);
	maxHpRectXSize = playerHpRect->getSize().x;

	playerInfoText->setPosition(playerInfoPanel->getGlobalBounds().position);
	
	battleHUD.push_back(playerHpRect);
	battleHUD.push_back(playerInfoPanel);


	//enemy control panel
	enemyInfoPanel = std::make_shared<sf::RectangleShape>();
	enemyInfoPanel->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 700.f , (m_window->getSize().y / 1080.f) * 300.f });
	enemyInfoPanel->setFillColor(sf::Color{ 128,128,128,50 });
	enemyInfoPanel->setPosition(sf::Vector2f{ m_window->getSize().x * 0.05f, m_window->getSize().y * 0.05f });

	enemyInfoText->setPosition(enemyInfoPanel->getGlobalBounds().position);

	battleHUD.push_back(enemyInfoPanel);


}

void BattleCore::initText()
{
	std::string info;

	if (!mainFont.openFromFile("Assets/Fonts/BigShouldersStencil.ttf"))
	{
		std::cout << "ERROR::LOADFROMFILE::BigShouldersStencil.ttf";
	}

	playerInfoText = std::make_shared<sf::Text>(mainFont);
	info = entity1->creatureList[0]->m_name + ", Level " +
		std::to_string(entity1->creatureList[0]->getComponent<CStats>().m_level) + "\n\nHP: " +
		std::to_string(entity1->creatureList[0]->getComponent<CStats>().m_hp) +
		"\n\nMana: " + std::to_string(entity1->creatureList[0]->getComponent<CStats>().m_mana);
	playerInfoText->setString(info);
	playerInfoText->setCharacterSize(36);

	textHUD.push_back(playerInfoText);

	enemyInfoText = std::make_shared<sf::Text>(mainFont);
	info = entity2->creatureList[0]->m_name + ", Level " +
		std::to_string(entity2->creatureList[0]->getComponent<CStats>().m_level) + "\n\nHP: " +
		std::to_string(entity2->creatureList[0]->getComponent<CStats>().m_hp) +
		"\n\nMana: " + std::to_string(entity2->creatureList[0]->getComponent<CStats>().m_mana);
	enemyInfoText->setString(info);
	enemyInfoText->setCharacterSize(36);

	textHUD.push_back(enemyInfoText);

}

void BattleCore::render()
{

	m_window->clear();

	for (const auto& element : battleHUD)
	{
		m_window->draw(*element);
	}

	for (const auto& text : textHUD)
	{
		m_window->draw(*text);
	}

	m_window->display();

}

void BattleCore::handleEvents(sf::Event evt)
{
	if (evt.is<sf::Event::Closed>())
	{
		m_window->close();
	}
}

void BattleCore::update()
{
	if (playerTurn)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			friendlyCreature->attack(enemyCreature);
		}
	}

}


BattleCore::BattleCore(sf::RenderWindow* window, person* person1, person* person2)
{

	entity1 = person1;
	entity2 = person2;

	friendlyCreature = entity1->creatureList[0];
	enemyCreature = entity2->creatureList[0];
	playerTurn = true;

	m_window = window;

	initText();
	initBaseScene();

	update();

}
