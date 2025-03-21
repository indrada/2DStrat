#ifndef scenes_h
#define scenes_h
#include "worldMap.h"
#include "task.h"
#include "tile.h"
#include "person.h"
#include "resource.h"
#include "buttons.h"
#include "battle.h"

#include <ctime>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <inttypes.h>

class Scene
{
	public:
		Scene();
		virtual void handleEvent(sf::Event event)=0;
		virtual void renderFrame()=0;
	
};

class worldMapScene : virtual public Scene
{
	public:
		std::string selectedPersonString;
		std::string selectedTileString;
		std::string infoString;
		sf::Font *font;
		sf::Text *infoOverlay;
		sf::Text *selectedPersonOverlay;
		sf::Text *selectedTileOverlay;
		tile * hoveredTile;
		tile * selectedTile;

		person* selectedPerson;

		person* newPerson;
		person* oneEnemy;

		moveTask * taskToAdd;
		ButtonPanel * buttonPanel;
		bool endingTurn;
		worldMap * map;
		sf::RenderWindow * window;
		sf::Vector2f size;

		worldMapScene(sf::RenderWindow * window);

		void handleEvent(sf::Event event);
		void renderFrame();

		tile * getTileAtMousePosition(worldMap map, sf::Vector2f size);		
		std::string getTileString(worldMap map, tile * currentTile);
		std::string getInfoString(worldMap map, tile * currentTile);
		std::string getSelectedTileString(worldMap map, tile * currentTile);
		std::string getSelectedPersonString(person * selectedPerson);

		std::pair<person*, person*> getBattleOponents();

		void endTurn(worldMap *map);
};

class BattleScene
{

private:

	sf::RenderWindow* m_window;

	std::shared_ptr<BattleCore> m_BattleCore;

public:

	BattleScene(sf::RenderWindow* window, person*, person*);

	void handleEvent(sf::Event);

	void renderFrame();
	void updateScene();
};


#endif