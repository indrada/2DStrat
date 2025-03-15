#include "worldMap.h"
#include "task.h"
#include "tile.h"
#include "person.h"
#include "water.hpp"
#include "resource.h"
#include <ctime>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <inttypes.h>





void drawRectangles(sf::RenderWindow& win, worldMap toDisplay)
{
    for (int i = 0; i < toDisplay.horizontalSize * toDisplay.verticalSize; i++)
    {
        win.draw(toDisplay.rectangles[i]);
    }
}

tile* getTileAtMousePosition(worldMap map, sf::Vector2f size)
{

    auto mousePos = sf::Mouse::getPosition();
    int xPos = (int)(map.horizontalSize * mousePos.x / size.y);
    int yPos = (int)(map.verticalSize * mousePos.y / size.y);
    return map.tileAt(xPos, yPos);
}

std::string getTileString(worldMap map, tile * currentTile)
{
    std::string retval = "Elevation: " + std::to_string(currentTile->elevation);
    retval += "\nWater Depth: " + std::to_string(currentTile->waterDepth());
    if (map.resourceNames.size() > 0)
    {
        retval += "\nResources:";
        for (int i = 0; i < map.resourceNames.size(); i++)
        {
            retval += "\n  " + map.resourceNames.at(i) +": " + std::to_string(currentTile->resourceQuantity.at(i));
        }
    }
    return retval;
}

std::string getInfoString(worldMap map, tile * currentTile)
{
	return "Hovered Tile: " + getTileString(map,currentTile);
}

std::string getSelectedTileString(worldMap map, tile * currentTile)
{
	if(currentTile == NULL) return std::string("No Tile Selected");
	return "Selected Tile: \n" + getTileString(map,currentTile);
}




std::string getSelectedPersonString(person * selectedPerson)
{
	if(selectedPerson == NULL) return (std::string) "No Person Selected";
	return "Selected Person:\nName: " + selectedPerson->name + "\nHealth: " + std::to_string(selectedPerson->attributes->health)+ "\nMana: " + std::to_string(selectedPerson->attributes->mana)+ "\nStamina: " + std::to_string(selectedPerson->attributes->stamina)+ "\nFood: " + std::to_string(selectedPerson->attributes->food);
}

void endTurn(worldMap map)
{
	map.doTasks(1000);
	map.updateAttributes();
}

int main()
{
    srand(time(0));
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Testing",sf::Style::None);
    window.setFramerateLimit(144);
    mapMode* myMapMode = new defaultMap();
    worldMap myMap(window, 100, 100, myMapMode, 10.0f);
    rain(myMap, 0.5f);
    myMap.getRectangles(window, *myMapMode);
    Resource iron("iron", 1.0f);
    iron.registerResource(&myMap);
    person * newPerson= new person(20, 20, &myMap);
    newPerson->addPerson();
	person * person2 = new person(30,30,&myMap, "Jane Doe");
	person2->addPerson();
	printf("Here1");
    auto size = window.getView().getSize();
    sf::Font font("Assets/arial.ttf");
    sf::Text infoOverlay(font);
    infoOverlay.setCharacterSize(120);
    infoOverlay.setFillColor(sf::Color::White);
    infoOverlay.setOutlineColor(sf::Color::Black);
    infoOverlay.setOutlineThickness(3);
    infoOverlay.setStyle(sf::Text::Bold);
    infoOverlay.setPosition({0.57f*size.x,0.0f});
    infoOverlay.setScale({0.25f,0.25f});
    std::string infoString = "";
    infoOverlay.setString(infoString);
	printf("Here3");
	
    sf::Text selectedTileOverlay(font);
    selectedTileOverlay.setCharacterSize(120);
    selectedTileOverlay.setFillColor(sf::Color::White);
    selectedTileOverlay.setOutlineColor(sf::Color::Black);
    selectedTileOverlay.setOutlineThickness(3);
    selectedTileOverlay.setStyle(sf::Text::Bold);
    selectedTileOverlay.setPosition({0.57f*size.x,0.33f*size.y});
    selectedTileOverlay.setScale({0.25f,0.25f});
    std::string selectedTileString = "";
    selectedTileOverlay.setString(selectedTileString);
	printf("Here4");
	
	sf::Text selectedPersonOverlay(font);
    selectedPersonOverlay.setCharacterSize(120);
    selectedPersonOverlay.setFillColor(sf::Color::White);
    selectedPersonOverlay.setOutlineColor(sf::Color::Black);
    selectedPersonOverlay.setOutlineThickness(3);
    selectedPersonOverlay.setStyle(sf::Text::Bold);
    selectedPersonOverlay.setPosition({0.57f*size.x,0.67f*size.y});
    selectedPersonOverlay.setScale({0.25f,0.25f});
    std::string selectedPersonString = "";
    selectedPersonOverlay.setString(selectedPersonString);
	printf("Here5");
	
    tile * hoveredTile = getTileAtMousePosition(myMap, size);
    tile * selectedTile = NULL;
    person* selectedPerson = NULL;
    moveTask * taskToAdd=NULL;
	bool endingTurn = false;
    while (window.isOpen())
    {
		printf("Here6");
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (event->getIf< sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                    selectedTile = getTileAtMousePosition(myMap, size);
                    selectedPerson = selectedTile->personHere;
					if(selectedPerson == NULL)
					{
						printf("Selected Tile at %d, %d\n", selectedTile->xPos, selectedTile->yPos);  
					}
					else
					{
						printf(("Selected " + selectedPerson->name + "\n").c_str());
					}
                }
            }

        }
		printf("Here7");
        if (selectedPerson != NULL)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
				taskToAdd = new moveTask(1, selectedPerson, WEST);
                selectedPerson->addTask(taskToAdd);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
				taskToAdd = new moveTask(1, selectedPerson, EAST);
                selectedPerson->addTask(taskToAdd);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
				taskToAdd = new moveTask(1, selectedPerson, NORTH);
                selectedPerson->addTask(taskToAdd);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
				taskToAdd = new moveTask(1, selectedPerson, SOUTH);
                selectedPerson->addTask(taskToAdd);
            }
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
			{
				if(!endingTurn){
					printf("Turn Ended, Doing Tasks...\n");
					myMap.doTasks(5);	
				}
				endingTurn = true;
			}
			else
			{
				endingTurn = false;
			}
        }

		printf("Here8");
        drawRectangles(window, myMap);
        hoveredTile = getTileAtMousePosition(myMap, size);
        infoString = getInfoString(myMap,hoveredTile);
        infoOverlay.setString(infoString);
		printf("Here9");
		std::string selectedTileString = getSelectedTileString(myMap,selectedTile);
		printf("Here10");
		selectedTileOverlay.setString(selectedTileString);
		std::string selectedPersonString = getSelectedPersonString(selectedPerson);
		selectedPersonOverlay.setString(selectedPersonString);
		
        window.draw(infoOverlay);
		window.draw(selectedTileOverlay);
		window.draw(selectedPersonOverlay);
		
        window.display();
        window.clear();

    }
}

