#include "buttons.h"
#include <sfml/graphics.hpp>
#include <sfml/window.hpp>
#include <SFML/System/Vector2.hpp>

ButtonPanel::ButtonPanel()
{
	
}

ButtonPanel::ButtonPanel(sf::Vector2<float> position) : position(position)
{
	
}

void ButtonPanel::processButtons()
{
	for(Button * button : buttonList)
	{
		auto mouse_pos = sf::Mouse::getPosition(*(button->window));
		if(button->sprite->getGlobalBounds().contains(sf::Vector2<float>((float)mouse_pos.x,(float)mouse_pos.y)))
		{
			printf("Here");
			button->process();
		}
		button->window->draw(*(button->sprite));
	}
}

void ButtonPanel::addButton(Button * button)
{
	button->position = {position.x,position.y+100*buttonList.size()};
	buttonList.push_back(button);
	button->sprite->setPosition(button->position);
	printf("Button Position is %f, %f\n",buttonList.front()->sprite->getPosition().x,buttonList.front()->sprite->getPosition().y);
}


Button::Button(std::string fileName, sf::RenderWindow * window)
{
	this->window = window;
	sf::Texture * texture = new sf::Texture();
	if(!texture->loadFromFile("Assets/Icons/"+fileName))
	{
		texture->loadFromFile("Assets/Icons/no-image.png");
	}
	printf(("Assets/Icons/"+fileName).c_str());
	sprite = new sf::Sprite(*texture);
}

ElevationMapButton::ElevationMapButton(std::string fileName, sf::RenderWindow * window, worldMap * map) : Button(fileName, window)
{
	this->map = map;
}
void ElevationMapButton::process()
{
	map->updateMapMode(new defaultMap());
}

ResourceMapButton::ResourceMapButton(std::string fileName, sf::RenderWindow * window, worldMap * map, int resourceIndex) : Button(fileName, window)
{
	this->map = map;
	this->resourceIndex = resourceIndex;
}
void ResourceMapButton::process()
{
	map->updateMapMode(new resourceMap(resourceIndex));
}