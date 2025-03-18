#include "worldMap.h"
#include "task.h"
#include "tile.h"
#include "person.h"
#include "water.hpp"
#include "resource.h"
#include "buttons.h"
#include "battle.h"
#include "scenes.h"
#include <ctime>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <inttypes.h>
#include <SFML/Graphics/Text.hpp>

Scene::Scene()
{
	
}

tile* worldMapScene::getTileAtMousePosition(worldMap map, sf::Vector2f size)
{
    auto mousePos = sf::Mouse::getPosition();
    int xPos = (int)(map.horizontalSize * mousePos.x / size.y);
    int yPos = (int)(map.verticalSize * mousePos.y / size.y);
    return map.tileAt(xPos, yPos);
}

std::string worldMapScene::getTileString(worldMap map, tile * currentTile)
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

std::string worldMapScene::getInfoString(worldMap map, tile * currentTile)
{
	return "Hovered Tile: \n" + getTileString(map,currentTile);
}

std::string worldMapScene::getSelectedTileString(worldMap map, tile * currentTile)
{
	if(currentTile == nullptr) return std::string("No Tile Selected");
	return "Selected Tile: \n" + getTileString(map,currentTile);
}




std::string worldMapScene::getSelectedPersonString(person * selectedPerson)
{
	if(selectedPerson == nullptr) return (std::string) "No Person Selected";
	return "Selected Person:\nName: " + selectedPerson->name + "\nHealth: " + std::to_string(selectedPerson->attributes->health)+ "\nMana: " + std::to_string(selectedPerson->attributes->mana)+ "\nStamina: " + std::to_string(selectedPerson->attributes->stamina)+ "\nFood: " + std::to_string(selectedPerson->attributes->food);
}

void worldMapScene::endTurn(worldMap *map)
{
	map->doTasks(1000);
	map->updateAttributes();
}		

void worldMapScene::handleEvent(sf::Event event)
{
	if (event.is<sf::Event::Closed>())
	{
		window->close();
	}
	if (event.is<sf::Event::MouseButtonPressed>())
	{
		if (event.getIf< sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
		{
			selectedTile = getTileAtMousePosition(*map, size);
			selectedPerson = selectedTile->personHere;
			if (selectedPerson == nullptr)
			{
				printf("Selected Tile at %d, %d\n", selectedTile->xPos, selectedTile->yPos);
			}
			else
			{
				printf(("Selected " + selectedPerson->name + "\n").c_str());
			}
		}
	}

	else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (selectedPerson != nullptr)
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
			{
				taskToAdd = new moveTask(1, selectedPerson, WEST);
				selectedPerson->addTask(taskToAdd);
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
			{
				taskToAdd = new moveTask(1, selectedPerson, EAST);
				selectedPerson->addTask(taskToAdd);
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
			{
				taskToAdd = new moveTask(1, selectedPerson, NORTH);
				selectedPerson->addTask(taskToAdd);
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
			{
				taskToAdd = new moveTask(1, selectedPerson, SOUTH);
				selectedPerson->addTask(taskToAdd);
			}
		}
	}
}

void worldMapScene::renderFrame()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
	{
		if(!endingTurn)
		{
			endTurn(map);
		}
		endingTurn = true;
	}
	else
	{
		endingTurn = false;
	}
	hoveredTile = getTileAtMousePosition(*map, size);
	infoString = getInfoString(*map,hoveredTile);
	infoOverlay->setString(infoString);
	selectedTileString = getSelectedTileString(*map,selectedTile);
	selectedTileOverlay->setString(selectedTileString);
	selectedPersonString = getSelectedPersonString(selectedPerson);
	selectedPersonOverlay->setString(selectedPersonString);
	window->draw(*infoOverlay);
	window->draw(*selectedTileOverlay);
	window->draw(*selectedPersonOverlay);
	window->draw(map->triangles);
	buttonPanel->processButtons();
	
	window->display();
	window->clear();
}

worldMapScene::worldMapScene(sf::RenderWindow * window)
{	
    srand(time(0));	
	this->window = window;
    mapMode* myMapMode = new defaultMap();
    map = new worldMap(window, 100, 120, myMapMode, 10.0f);	
    rain(*map, 0.5f);
    Resource iron("iron", 1.0f);
    iron.registerResource(map);	
    person * newPerson= new person(5, 5, map);
    newPerson->addPerson();
	person * person2 = new person(5,5,map, "Jane Doe");
	person2->addPerson();	
    map->generateVertexArray();
    size = window->getView().getSize();	
    font = new sf::Font("Assets/Fonts/arial.ttf");
    infoOverlay = new sf::Text(*font);
	infoOverlay->setCharacterSize(120);
    infoOverlay->setFillColor(sf::Color::White);
    infoOverlay->setOutlineColor(sf::Color::Black);
    infoOverlay->setOutlineThickness(3);
    infoOverlay->setStyle(sf::Text::Bold);
    infoOverlay->setPosition({0.57f*size.x,0.0f});
    infoOverlay->setScale({0.25f,0.25f});
    infoString = "";
    infoOverlay->setString(infoString);
	
    selectedTileOverlay = new sf::Text(*font);
    selectedTileOverlay->setCharacterSize(120);
    selectedTileOverlay->setFillColor(sf::Color::White);
    selectedTileOverlay->setOutlineColor(sf::Color::Black);
    selectedTileOverlay->setOutlineThickness(3);
    selectedTileOverlay->setStyle(sf::Text::Bold);
    selectedTileOverlay->setPosition({0.57f*size.x,0.33f*size.y});
    selectedTileOverlay->setScale({0.25f,0.25f});
    selectedTileString = "";
    selectedTileOverlay->setString(selectedTileString);
	
	selectedPersonOverlay = new sf::Text(*font);
    selectedPersonOverlay->setCharacterSize(120);
    selectedPersonOverlay->setFillColor(sf::Color::White);
    selectedPersonOverlay->setOutlineColor(sf::Color::Black);
    selectedPersonOverlay->setOutlineThickness(3);
    selectedPersonOverlay->setStyle(sf::Text::Bold);
    selectedPersonOverlay->setPosition({0.57f*size.x,0.67f*size.y});
    selectedPersonOverlay->setScale({0.25f,0.25f});
    selectedPersonString = "";
    selectedPersonOverlay->setString(selectedPersonString);	
	
	buttonPanel = new ButtonPanel({size.x-100,0.0f});
	buttonPanel->addButton(new ResourceMapButton("ore.png", window, map,0));
	hoveredTile = getTileAtMousePosition(*map, size);
    selectedTile = nullptr;
    selectedPerson = nullptr;
    taskToAdd=nullptr;
	endingTurn = false;
}
        
		


