#include "abilities.h"
#include "person.h"

// Core ability
AbilityCore::AbilityCore(std::shared_ptr<Creature> self, std::string name)
	: m_name(name)
{
	m_self = self;
}

bool AbilityCore::execute(std::shared_ptr<Creature> creature)
{
	std::cout << "Basic execute" << '\n';
	return true;
}


// Attack ability
AttackAbility::AttackAbility(std::shared_ptr<Creature> self, std::string name, int damage, int mcost)
	: AbilityCore(self, name), m_damage(damage), m_manacost(mcost) {}

bool AttackAbility::execute(std::shared_ptr<Creature> creature)
{
	std::cout << "Attack execute " + m_name << '\n';

	if (m_self->getComponent<CStats>().m_mana < m_manacost) { return false; }

	creature->takeDamage(m_damage);
	m_self->getComponent<CStats>().m_mana -= m_manacost;

	return true;
	
}

Buff::Buff(std::shared_ptr<Creature> self, std::string name, int duration)
	: m_self(self), m_name(name), m_duration(duration)
{
	m_type = BUFFTYPE::NONE;
}

bool Buff::checkDuration()
{
	m_duration--;
	std::cout << m_name << " is " << m_duration << " turn left\n";
	if (m_duration == 0)
	{
		discardBuff();
		return true;
	}
	return false;
}

void Buff::applyBuff()
{
	std::cout << "Default buff apply\n";
}

void Buff::discardBuff()
{
	std::cout << "Default buff discard\n";
}

BUFFTYPE Buff::getType() const
{
	return m_type;
}


DefenceUpBuff::DefenceUpBuff(std::shared_ptr<Creature> self, std::string name, int duration)
	:Buff(self,name,duration) 
{
	m_type = BUFFTYPE::PASSIVE;
	applyBuff();
}

void DefenceUpBuff::applyBuff()
{
	m_self->getComponent<CBattleStats>().m_savedDefence = m_self->getComponent<CBattleStats>().m_defence;
	m_self->getComponent<CBattleStats>().m_defence *= 3;
}

void DefenceUpBuff::discardBuff()
{
	m_self->getComponent<CBattleStats>().m_defence = m_self->getComponent<CBattleStats>().m_savedDefence;
}
