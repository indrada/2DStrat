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
