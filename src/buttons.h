#ifndef buttons_h
#define buttons_h
#include "worldMap.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Button
{
	public:
		sf::Vector2<float> position;
		sf::Sprite * sprite;
		sf::RenderWindow * window;
		Button(std::string fileName, sf::RenderWindow * window);
		virtual void process() = 0;
};


class ButtonPanel
{
	public:
		std::vector<Button *> buttonList;
		sf::Vector2<float> position;
		ButtonPanel();
		ButtonPanel(sf::Vector2<float> position);
		void addButton(Button * button);
		void processButtons(sf::Vector2i mouse_pos);
		void renderButtons();
};


class MapButton : virtual public Button
{
	public:
		worldMap * map;
		mapMode * mode;
		int resourceIndex;
		MapButton(std::string fileName, sf::RenderWindow * window, worldMap * map, mapMode * mode);
		void process() override;
};

class StartGameButton : virtual public Button
{
	public:
		StartGameButton(std::string fileName, sf::RenderWindow * window);
		void process() override;
};

#endif