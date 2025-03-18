#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <deque>
#include "worldMap.h"
#include "task.h"
#include "components.h"
#include <tuple>

// also CSprite, CActions and etc
typedef std::tuple<CStats> componetsTuple;

class attribute
{
    public:
        int health;
        int maxHealth;
		int food;
		int stamina;
		int mana;
		attribute(int health = 100, int food = 100, int stamina = 100, int mana = 100);
};

class Creature
{

private:

    // maybe add some tags to them
    componetsTuple components;

public:

    std::string m_name;

    Creature(std::string name)
        :m_name(name){}

    virtual void doTurn(person* enemy, person* friendly) = 0;

    template<typename T>
    T& getComponent()
    {
        return std::get<T>(components);
    }

    template<typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    //This template function is used for creating new components for our creatures
    // TArgs is just arguments for component constructor
    template<typename T, typename ... TArgs>
    T& addComponent(TArgs&& ... mArgs)
    {
        auto component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;

    }


};


class Vampire : public Creature
{
	
public:

    Vampire(std::string);

    void doTurn(person* enemy, person* friendly) override;

};

class person {
    public:
        int xPos;
        int yPos;
		bool isFriendly;
        std::deque<task*> tasks;
        worldMap* attachedMap;
		std::string name;
		std::vector<std::shared_ptr<Creature>> creatureList;
		attribute * attributes;

        person();
        person(int xPos, int yPos, worldMap* attachedMap,std::string name = "John Doe");

        void addTask(task* toAdd);
        bool doTasks(int Time);
        void addPerson();
        void addCreature();
		void updateAttributes();


};

#endif