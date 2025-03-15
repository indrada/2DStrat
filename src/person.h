#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <deque>
#include "worldMap.h"
#include "task.h"

class attribute
{
    public:
        int health;
		int food;
		int stamina;
		int mana;
		attribute(int health = 100, int food = 100, int stamina = 100, int mana = 100);
};

class person {
    public:
        int xPos;
        int yPos;
        std::deque<task*> tasks;
        worldMap* attachedMap;
		std::string name;
		attribute * attributes;
        person();
        person(int xPos, int yPos, worldMap* attachedMap,std::string name = "John Doe");
        void addTask(task* toAdd);
        bool doTasks(int Time);
        void addPerson();
		void updateAttributes();
};

#endif