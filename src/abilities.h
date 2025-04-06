#pragma once

#include <memory>
#include <string>
#include <iostream>

class Creature;


class AbilityCore
{

protected:

	std::shared_ptr<Creature> m_self;

public:

	std::string m_name;

	AbilityCore(std::shared_ptr<Creature> self, std::string name);

	virtual bool execute(std::shared_ptr<Creature> creature);

};

class AttackAbility : public AbilityCore
{

private:

	int m_damage, m_manacost;

public:

	AttackAbility(std::shared_ptr<Creature> self, std::string name, int damage, int mcost);

	bool execute(std::shared_ptr<Creature> creature);

};

class Buff
{

private:

	int m_duration;
	std::string m_name;

	std::shared_ptr<Creature> m_self;

public:

	Buff(std::shared_ptr<Creature> self, std::string name, int duration);

	bool checkDuration();

	virtual void applyBuff();
	virtual void discardBuff();

	bool operator==(const Buff& buff);

};

