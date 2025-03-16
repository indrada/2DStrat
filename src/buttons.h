#ifndef buttons_h
#define buttons_h
#include "worldMap.h"
#include <sfml/graphics.hpp>
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
		void processButtons();
};

class ElevationMapButton : virtual public Button
{
	public:
		worldMap * map;
		ElevationMapButton(std::string fileName, sf::RenderWindow * window, worldMap * map);
		void process() override;
};

class ResourceMapButton : virtual public Button
{
	public:
		worldMap * map;
		int resourceIndex;
		ResourceMapButton(std::string fileName, sf::RenderWindow * window, worldMap * map, int resourceIndex);
		void process() override;
};


#endif