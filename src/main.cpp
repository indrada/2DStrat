#include "worldMap.h"
#include "context.h"
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

struct globalContext context;
int main()
{
    // it's not ideal solution but for testing is okey

    
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Testing", sf::State::Fullscreen);
    window->setFramerateLimit(144);	
    //change when commit

    Scene * scene = new worldMapScene(window);
    //just change this to test world stuff
    context.scene = scene;
    context.window = window;

    while (window->isOpen())
    {

        while (const std::optional<sf::Event> event = window->pollEvent())
		{
            context.scene->handleEvent(event.value());            
		}        
        context.scene->renderFrame();           
    }
}

