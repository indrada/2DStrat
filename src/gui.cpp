#include "gui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include "context.h"

extern globalContext context;

//ButtonPanel
gui::ButtonPanel::ButtonPanel() {}

gui::ButtonPanel::ButtonPanel(sf::Vector2<float> position) : position(position) {}

void gui::ButtonPanel::processButtons(sf::Vector2i mouse_pos)
{
	printf("ButtonList Size in processButtons = %d", buttonList.size());
	for (Button* button : buttonList)
	{
		if (button->sprite->getGlobalBounds().contains(sf::Vector2<float>((float)mouse_pos.x, (float)mouse_pos.y)))
		{
			printf("Here");
			button->process();
		}
	}
}

void gui::ButtonPanel::renderButtons()
{
	for (Button* button : buttonList)
	{
		button->window->draw(*(button->sprite));
	}
}

void gui::ButtonPanel::addButton(Button* button)
{
	printf("ButtonList Size = %d\n", buttonList.size());
	button->position = { position.x,position.y + 100 * buttonList.size() };
	button->sprite->setPosition(button->position);
	buttonList.push_back(button);
	printf("Button Position is %f, %f\n", buttonList.back()->sprite->getPosition().x, buttonList.back()->sprite->getPosition().y);
}


//Button
gui::Button::Button(std::string fileName, sf::RenderWindow* window)
{
	this->window = window;
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("Assets/Icons/" + fileName))
	{
		texture->loadFromFile("Assets/Icons/no-image.png");
	}
	printf(("Assets/Icons/" + fileName).c_str());
	sprite = new sf::Sprite(*texture);
}


//MapButton
gui::MapButton::MapButton(std::string fileName, sf::RenderWindow* window, worldMap* map, mapMode* mode) : Button(fileName, window)
{
	this->map = map;
	this->resourceIndex = resourceIndex;
	this->mode = mode;
}

void gui::MapButton::process()
{
	map->updateMapMode(mode);
}


//StartGameButton
void gui::StartGameButton::process()
{
	context.scene = new worldMapScene(context.window, 100, 100, 2);
}

gui::StartGameButton::StartGameButton(std::string fileName, sf::RenderWindow* window) : Button(fileName, window)
{
	this->window = window;
}


//ActionsMenu
gui::ActionsMenu::ActionsMenu(sf::Vector2f pos, sf::Vector2f size)
{
	menuFont = std::make_unique<sf::Font>("Assets/Fonts/BigShouldersStencil.ttf");
	if (!menuFont)
	{
		std::cout << "ERROR::LOADFROMFILE::ActionsMenu::menuFont\n";
	}

	backgroundRect.setSize(size);
	backgroundRect.setPosition(pos);
	backgroundRect.setFillColor(sf::Color{ 128,128,128,50 });

}

void gui::ActionsMenu::draw(sf::RenderWindow* window)
{
	window->draw(backgroundRect);

	for (const auto& text : textActions)
	{
		window->draw(text);
	}

}

void gui::ActionsMenu::addAction(std::string action)
{
	sf::Text temp(*menuFont);
	std::string totalString{ std::to_string((textActions.size() + 1)) + ") "};
	totalString += action;
	temp.setString(totalString);
	temp.setCharacterSize(24);

	textActions.push_back(temp);

	int index = 1;
	for (auto& action : textActions)
	{
		action.setPosition(sf::Vector2f{ backgroundRect.getPosition().x + 0.05f * backgroundRect.getPosition().x,
			backgroundRect.getPosition().y + (index - 1) *
			(backgroundRect.getGlobalBounds().size.y / textActions.size()) });

		index++;
	}


}
