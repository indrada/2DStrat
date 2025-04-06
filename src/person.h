#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <deque>
#include <tuple>
#include <iostream>

#include "task.h"
#include "worldMap.h"
#include "components.h"
#include "abilities.h"


// also CSprite, CActions and etc
typedef std::tuple<CStats, CBattleStats> componetsTuple;

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

protected:

    // maybe add some tags to them
    componetsTuple components;


public:

    std::vector<AttackAbility> attackAbilities;
    std::vector<Buff> buffs;

    std::string m_name;
    

    Creature(std::string name)
        :m_name(name){}

    template<typename T>
    T& getComponent()
    {
        return std::get<T>(components);
    }

    template<typename T>
    const T& getComponent() const
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
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;

    }


    void addAtkAbility(AttackAbility ability)
    {
        attackAbilities.push_back(ability);
    }

    void addBuff(Buff buff)
    {
        buffs.push_back(buff);
    }

    void deleteBuff(Buff& buff)
    {
        std::remove(buffs.begin(), buffs.end(), buff);
        buffs.pop_back();
    }

    void attack(std::shared_ptr<Creature> enemy)
    {
        // k - armor effectivness, maybe set it based on types of creatures armor
        int k = 100;
        int damage = this->getComponent<CBattleStats>().m_damage * (1 - (enemy->getComponent<CBattleStats>().m_defence/(enemy->getComponent<CBattleStats>().m_defence + k)));
        enemy->takeDamage(damage);
    }
    
    void takeDamage(int damage)
    {
        std::cout << "Attacked with " << damage << '\n';
        this->getComponent<CStats>().m_hp -= damage;
    }

    bool isAlive()
    {
        return this->getComponent<CStats>().m_hp > 0;
    }


};


class Vampire : public Creature
{
	
public:

    Vampire(std::string);

};

class person {
    public:
        int xPos;
        int yPos;
        int dstXPos;
        int dstYPos;
        std::deque<task*> tasks;
        worldMap* attachedMap;
		std::string name;
		std::vector<std::shared_ptr<Creature>> creatureList;
		attribute * attributes;
        
        person();
        person(int xPos, int yPos, worldMap* attachedMap,std::string name = "John Doe", bool isFriendly = true);

        void addTask(task *toAdd);
        bool doTasks(int Time);
        void addPerson();
        void addCreature();
		void updateAttributes();
        void setIsFriendly(bool value);
        bool getIsFriendly();
        bool isAlive();
        void moveTo(int x, int y);
        void clearTasks();

        //getters
        int getXPos();
        int getYPos();
        
    private:
        bool isFriendly;
        
        
    };

#endif