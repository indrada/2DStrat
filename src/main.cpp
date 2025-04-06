#include "worldMap.h"
#include "context.h"
#include "task.h"
#include "tile.h"
#include "person.h"
#include "resource.h"
#include "gui.h"
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

    
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Testing", sf::State::Fullscreen);
    window->setFramerateLimit(144);	
    window->setKeyRepeatEnabled(false);
    //change when commit
    context.window = window;

    Scene * scene = new menuScene(window);

    context.scene = scene;
    while (window->isOpen())
    {

        while (const std::optional<sf::Event> event = window->pollEvent())
		{
            context.scene->handleEvent(event.value());
            printf("\nEvent Handled\n");
		}        
        printf("\nAll Events Handled for this Frame\n");
        context.scene->renderFrame();
        printf("Frame Rendered");
    }
}

