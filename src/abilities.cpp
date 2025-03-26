#include "abilities.h"

abilities::AttackAbility::AttackAbility(std::string name, int dmg, int manaCost)
	:m_name(name), m_damage(dmg), m_manaCost(manaCost) {}


void abilities::AttackAbility::execute()
{
	std::cout << "Used " + m_name << '\n';
}
