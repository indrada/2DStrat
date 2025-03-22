#include "person.h"
#include "task.h"
#include <vector>


person::person()
{
}

person::person(int xPos, int yPos, worldMap * attachedMap, std::string name, bool isFriendly)
{
	this->attachedMap = attachedMap;
	this->xPos = xPos;
	this->yPos = yPos;
	this->name = name;
	this->attributes = new attribute();
	this->isFriendly = isFriendly;
}

void person::addTask(task* toAdd)
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

//for testing
void person::addCreature()
{
	auto creature = std::make_shared<Vampire>("Vampire");
	creature->addComponent<CStats>(50, 15, 25);
	creature->addComponent<CBattleStats>(5, 5, ELEMENT::NONE);

	creatureList.push_back(creature);
}

void person::updateAttributes()
{
	attributes->stamina=std::min(attributes->stamina+8, 100);
	attributes->health=std::min(attributes->health+10, 100);
	attributes->mana=std::min(attributes->mana+10, 100);
	attributes->food=std::max(attributes->food-1,0);
}

attribute::attribute(int health, int food, int stamina, int mana)
{
	this->health = health;
	maxHealth = this->health;
	this->food = food;
	this->stamina = stamina;
	this->mana = mana;
}


Vampire::Vampire(std::string name)
	:Creature(name)
{}

void Vampire::doTurn(person * enemy, person * friendly)
{
	//do stuff
}

void person::setIsFriendly(bool value)
{
	this->isFriendly = value;
}

bool person::getIsFriendly()
{
	return this->isFriendly;
}

bool person::isAlive()
{
	printf("\nChecking aliveness");
	
	printf("\n%d",this);
	if(!creatureList.empty())
	{
		printf("\nPerson is Alive\n");
	}
	else
	{
		printf("\nPerson is Dead\n");
	}
	return !creatureList.empty();
}

