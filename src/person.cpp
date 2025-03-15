#include "person.h"
#include "task.h"
#include <vector>


person::person(int xPos, int yPos, worldMap * attachedMap, std::string name)
{
    this->attachedMap = attachedMap;
    this->xPos = xPos;
    this->yPos = yPos;
	this->name = name;
	this->attributes = new attribute();
}
void person::addTask(task * toAdd)
{
    tasks.push_back(toAdd);
}
bool person::doTasks(int Time)
{
	bool didTask = false;
    if (tasks.empty()) {
		printf((name + " had no tasks to complete\n").c_str());
		return false;
	}
    if (tasks.front()->timeToComplete <= Time)
    {
		didTask = true;
        if (tasks.front()->doTask())
        {
			tasks.pop_front();
			printf((name + " did a task that took ").c_str());
			printf("%d units of time\n", Time);
        }
        else
        {
            tasks.front()->timeToComplete = 1;
			printf((name + " failed at their task, and will try again\n").c_str());
        }
    }
    else
    {
        tasks.front()->timeToComplete -= Time;
    }
	return didTask;
}

void person::addPerson()
{
    attachedMap->tileAt(xPos, yPos)->personHere = this;
    attachedMap->updateRectangle(xPos, yPos);
	attachedMap->allPersons.push_back(this);
}

void person::updateAttributes()
{
	attributes->stamina+=8;
	attributes->health+=10;
	attributes->mana+=10;
	attributes->food-=1;
}

attribute::attribute(int health, int food, int stamina, int mana)
{
	this->health = health;
	this->food = food;
	this->stamina = stamina;
	this->mana = mana;
}