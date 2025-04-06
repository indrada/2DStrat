#include <vector>
#include "context.h"
#include "person.h"
#include "task.h"

extern globalContext context;

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
	this->dstXPos = xPos;
	this->dstYPos = yPos;
}

void person::addTask(task* toAdd)
{
    tasks.push_back(toAdd);
}

bool person::doTasks(int Time)
{
	bool didTask = false;
    if (tasks.empty()) {
		return false;
	}
    if (tasks.front()->timeToComplete <= Time)
    {
		didTask = true;
        if (tasks.front()->doTask())
        {
			delete (tasks.front());
			tasks.pop_front();
        }
        else
        {
            tasks.front()->timeToComplete = 1;
        }
    }
    else
    {
        tasks.front()->timeToComplete -= Time;
    }
	while (const std::optional<sf::Event> event = context.window->pollEvent())
	{
		context.scene->handleEvent(event.value());            
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
	creature->addComponent<CBattleStats>(10, 25, ELEMENT::NONE);

	AttackAbility temp(creature, "FireBall", 50, 25);
	AttackAbility temp2(creature, "Thunder", 25, 15);
	AttackAbility temp3(creature, "Mega bite", 15, 10);
	creature->addAtkAbility(temp);
	creature->addAtkAbility(temp2);
	creature->addAtkAbility(temp3);

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
	return !creatureList.empty();
}

void person::clearTasks()
{
	for(task * toClear : tasks)
	{
		delete toClear;
	}
	tasks.clear();
	dstXPos=xPos;
	dstYPos=yPos;
}

void person::moveTo(int x, int y)
{
	std::cout << x << " " << y << std::endl;
	while(x>dstXPos)
	{
		addTask(new moveTask(1,this,EAST));
		dstXPos++;
	}
	while(y>dstYPos)
	{
		addTask(new moveTask(1,this,SOUTH));
		dstYPos++;
	}
	while(x<dstXPos)
	{
		addTask(new moveTask(1,this,WEST));
		dstXPos--;
	}
	while(y<dstYPos)
	{
		addTask(new moveTask(1,this,NORTH));
		dstYPos--;
	}
}

int person::getXPos()
{
	return this->xPos;
}

int person::getYPos()
{
	return this->yPos;
}
