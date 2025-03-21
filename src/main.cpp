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
    // it's not ideal solution but for testing is okey
    enum class SCENES {WORLD, BATTLE};

    
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Testing", sf::State::Fullscreen);
    window->setFramerateLimit(144);	
    //change when commit

    worldMapScene scene(window);
    
    BattleScene scene2(window, scene.getBattleOponents().first, scene.getBattleOponents().second);

    //just change this to test world stuff
    SCENES currentScene = SCENES::BATTLE;

    while (window->isOpen())
    {

        while (const std::optional<sf::Event> event = window->pollEvent())
		{
            switch (currentScene)
            {
            case SCENES::WORLD:
                scene.handleEvent(event.value());
                break;
            case SCENES::BATTLE:
                scene2.handleEvent(event.value());
                break;
            default:
                break;
            }
		}
        
        switch (currentScene)
        {
        case SCENES::WORLD:         
            scene.renderFrame();
            break;
        case SCENES::BATTLE:
            scene2.updateScene();
            scene2.renderFrame();
            break;
        default:
            break;
        }
		
     
    }
}

