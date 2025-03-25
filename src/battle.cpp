#pragma once
extern struct globalContext context;
#include "battle.h"
#include "context.h"
void BattleCore::initBaseScene()
{

	// TODO : change 1920.f to screen resolutions 
	//change it to player and enemy sprites then
	playerHero = std::make_shared<sf::RectangleShape>();
	playerHero->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 200.f , (m_window->getSize().y / 1080.f) * 100.f });
	playerHero->setFillColor(sf::Color::Green);
	playerHero->setPosition(sf::Vector2f{ m_window->getSize().x * 0.2f, m_window->getSize().y * 0.8f });

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

	// hp bar
	playerHpBar = std::make_shared<gui::ProgressBar<float>>(sf::Vector2f{ playerInfoPanel->getPosition().x,
		playerInfoPanel->getPosition().y + playerInfoPanel->getSize().y * 0.45f },
		sf::Vector2f{ playerInfoPanel->getSize().x, 15.f });
	playerHpBar->setRange(0, friendlyCreature->getComponent<CStats>().maxHp);
	playerHpBar->changeValue(friendlyCreature->getComponent<CStats>().m_hp);

	// mana bar
	playerManaBar = std::make_shared<gui::ProgressBar<float>>(sf::Vector2f{ playerInfoPanel->getPosition().x,
		playerInfoPanel->getPosition().y + playerInfoPanel->getSize().y * 0.75f },
		sf::Vector2f{ playerInfoPanel->getSize().x, 15.f });
	playerManaBar->setRange(0, friendlyCreature->getComponent<CStats>().maxMana);
	playerManaBar->changeValue(friendlyCreature->getComponent<CStats>().m_mana);
	playerManaBar->setInlineColor(sf::Color::Blue);

	//Creature info text
	playerInfoText->setPosition(playerInfoPanel->getGlobalBounds().position);

	// Player actions menu
	playerActionMenu = std::make_shared<gui::ActionsMenu>(
		sf::Vector2f{playerInfoPanel->getGlobalBounds().position.x - 300,
		playerInfoPanel->getGlobalBounds().position.y},
		sf::Vector2f{300, playerInfoPanel->getGlobalBounds().size.y});
	playerActionMenu->addAction("Attack");
	playerActionMenu->addAction("Abilities");
	playerActionMenu->addAction("Defence");

	battleHUD.push_back(playerInfoPanel);


	//enemy control panel
	enemyInfoPanel = std::make_shared<sf::RectangleShape>();
	enemyInfoPanel->setSize(sf::Vector2f{ (m_window->getSize().x / 1920.f) * 700.f , (m_window->getSize().y / 1080.f) * 300.f });
	enemyInfoPanel->setFillColor(sf::Color{ 128,128,128,50 });
	enemyInfoPanel->setPosition(sf::Vector2f{ m_window->getSize().x * 0.05f, m_window->getSize().y * 0.05f });

	// hp bar
	enemyHpBar = std::make_shared<gui::ProgressBar<float>>(sf::Vector2f{ enemyInfoPanel->getPosition().x,
		enemyInfoPanel->getPosition().y + enemyInfoPanel->getSize().y * 0.45f },
		sf::Vector2f{ enemyInfoPanel->getSize().x, 15.f });
	enemyHpBar->setRange(0, enemyCreature->getComponent<CStats>().maxHp);
	enemyHpBar->changeValue(enemyCreature->getComponent<CStats>().m_hp);

	// mana bar
	enemyManaBar = std::make_shared<gui::ProgressBar<float>>(sf::Vector2f{ enemyInfoPanel->getPosition().x,
		enemyInfoPanel->getPosition().y + playerInfoPanel->getSize().y * 0.75f },
		sf::Vector2f{ playerInfoPanel->getSize().x, 15.f });
	enemyManaBar->setRange(0, enemyCreature->getComponent<CStats>().maxMana);
	enemyManaBar->changeValue(enemyCreature->getComponent<CStats>().m_mana);
	enemyManaBar->setInlineColor(sf::Color::Blue);

	enemyInfoText->setPosition(enemyInfoPanel->getGlobalBounds().position);

	battleHUD.push_back(enemyInfoPanel);


}

void BattleCore::initText()
{

	if (!mainFont.openFromFile("Assets/Fonts/BigShouldersStencil.ttf"))
	{
		std::cout << "ERROR::LOADFROMFILE::BigShouldersStencil.ttf";
	}

	playerInfoText = std::make_shared<sf::Text>(mainFont);
	playerInfoText->setCharacterSize(36);

	enemyInfoText = std::make_shared<sf::Text>(mainFont);
	enemyInfoText->setCharacterSize(36);

	updateInfoText();

	textHUD.push_back(playerInfoText);
	textHUD.push_back(enemyInfoText);

}

void BattleCore::updateInfoText()
{
	// player info
	std::string info = friendlyCreature->m_name + ", Level " +
		std::to_string(friendlyCreature->getComponent<CStats>().m_level) + "\n\nHP: " +
		std::to_string(friendlyCreature->getComponent<CStats>().m_hp) +
		"\n\nMana: " + std::to_string(friendlyCreature->getComponent<CStats>().m_mana);
	playerInfoText->setString(info);

	//enemy info
	info = enemyCreature->m_name + ", Level " +
		std::to_string(enemyCreature->getComponent<CStats>().m_level) + "\n\nHP: " +
		std::to_string(enemyCreature->getComponent<CStats>().m_hp) +
		"\n\nMana: " + std::to_string(enemyCreature->getComponent<CStats>().m_mana);
	enemyInfoText->setString(info);

}

void BattleCore::playerTurn()
{
	friendlyCreature->attack(enemyCreature);
	if (!(friendlyCreature->isAlive()))
	{
		entity1->creatureList.pop_back();
		if (entity1->creatureList.empty())
		{
			context.inBattle = false;
			return;
		}
		friendlyCreature = entity1->creatureList.back();
	}

	enemyHpBar->changeValue(enemyCreature->getComponent<CStats>().m_hp);

	isPlayerTurn = false;
	updateInfoText();
}

void BattleCore::enemyTurn()
{
	enemyCreature->attack(friendlyCreature);
	if (!(enemyCreature->isAlive()))
	{
		printf("You did it!\n");
		printf("%d\n", entity2->creatureList.size());
		entity2->creatureList.pop_back();
		if (entity2->creatureList.empty())
		{
			context.inBattle = false;
			return;
		}
		enemyCreature = entity2->creatureList.back();
	}
	isPlayerTurn = true;

	playerHpBar->changeValue(friendlyCreature->getComponent<CStats>().m_hp);
	updateInfoText();
}


void BattleCore::render()
{

	m_window->clear();

	playerHpBar->draw(m_window);
	enemyHpBar->draw(m_window);

	playerManaBar->draw(m_window);
	enemyManaBar->draw(m_window);

	for (const auto& element : battleHUD)
	{
		m_window->draw(*element);
	}

	for (const auto& text : textHUD)
	{
		m_window->draw(*text);
	}

	playerActionMenu->draw(m_window);

	m_window->display();

}

void BattleCore::handleEvents(sf::Event evt)
{
	if (evt.is<sf::Event::Closed>())
	{
		m_window->close();
	}
	if (const auto* keyPressed = evt.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::Enter)
		{
			playerTurn();
		}
		else if (keyPressed->code == sf::Keyboard::Key::Down)
		{
			playerActionMenu->changeIndex(1);
		}
		else if (keyPressed->code == sf::Keyboard::Key::Up)
		{
			playerActionMenu->changeIndex(-1);
		}
	}
}

void BattleCore::update()
{
	if (!isPlayerTurn)
	{
		enemyTurn();
	}

}


BattleCore::BattleCore(sf::RenderWindow* window, person* person1, person* person2)
{

	entity1 = person1;
	entity2 = person2;

	friendlyCreature = entity1->creatureList.back();
	enemyCreature = entity2->creatureList.back();
	isPlayerTurn = true;

	m_window = window;

	initText();
	initBaseScene();

	update();

}

bool initBattle(person* player, person* enemy)
{
	if (!player->isAlive()) return enemy->isAlive();
	printf("Here1111 A");
	if (!enemy->isAlive()) return false;
	context.inBattle = true;
	BattleCore battle(context.window, player, enemy);
	BattleScene scene(context.window, player, enemy);
	while (context.inBattle && context.window->isOpen())
	{

		while (const std::optional<sf::Event> event = context.window->pollEvent())
		{
			scene.handleEvent(event.value());
		}
		scene.renderFrame();
	}
	return enemy->isAlive();
}
