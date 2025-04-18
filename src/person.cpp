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
	this->assignedInventory = new inventory(30, 100, this);
	if(!isFriendly) attributes->stamina=10;
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
			didTask=false;
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

	auto temp = std::make_shared<AttackAbility>(creature, "FireBall", 25, 25);
	temp->m_description = "Throw fireball\n";
	auto temp2 = std::make_shared<AttackAbility>(creature, "Thunder", 15, 10);
	temp2->m_description = "Summon thunder\n";
	auto temp3 = std::make_shared<AttackAbility>(creature, "Kick", 5, 2);
	temp3->m_description = "Magic kick!\n";
	creature->addAtkAbility(temp);
	creature->addAtkAbility(temp2);
	creature->addAtkAbility(temp3);

	creatureList.push_back(creature);
}

void person::updateAttributes()
{
	attributes->stamina=std::min((int)(attributes->stamina+RandomNumber(0,20)), 100);
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

void person::addItem(item * toAdd)
{
	assignedInventory->addItem(toAdd);
}

void person::dig()
{
	float maxQuantity = -1.0f;
	int maxIndex = -1;
	getTile();
	printf("\nHere1\n");
	for (int i = 0; i < attachedMap->resourceNames.size(); i++)
	{
		if (getTile()->resourceQuantity[i] > maxQuantity)
		{
			maxQuantity = getTile()->resourceQuantity[i];
			maxIndex = i;
			printf("\nHere2\n");

		}
	}
	if (maxIndex >= 0) addItem(new item(1,1,((int)maxQuantity) + 1,attachedMap->resourceNames[maxIndex]));
	printf("\nHere3\n");
}
