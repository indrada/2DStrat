#include "gui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include "context.h"

extern globalContext context;

gui::ButtonPanel::ButtonPanel()
{

}

gui::ButtonPanel::ButtonPanel(sf::Vector2<float> position) : position(position)
{

}

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

void gui::StartGameButton::process()
{
	context.scene = new worldMapScene(context.window, 100, 100, 2);
}

gui::StartGameButton::StartGameButton(std::string fileName, sf::RenderWindow* window) : Button(fileName, window)
{
	this->window = window;
}