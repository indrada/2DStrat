#include "worldMap.h"
#include "task.h"
#include "tile.h"
#include "person.h"
#include "resource.h"
#include "buttons.h"
#include "battle.h"
#include "scenes.h"
#include <ctime>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <inttypes.h>

int main()
{
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Testing", sf::State::Fullscreen);
    window->setFramerateLimit(144);	
    worldMapScene scene(window);
    while (window->isOpen())
    {
        while (const std::optional<sf::Event> event = window->pollEvent())
		{
			scene.handleEvent(event.value());
		}
		scene.renderFrame();
     
    }
}

