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
};

class person {
    public:
        int xPos;
        int yPos;
        std::deque<task*> tasks;
        worldMap* attachedMap;
		std::string name;
        person();
        person(int xPos, int yPos, worldMap* attachedMap,std::string name = "John Doe");
        void addTask(task* toAdd);
        bool doTasks(int Time);
        void addPerson();
};

#endif