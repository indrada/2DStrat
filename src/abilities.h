#pragma once

#include <memory>
#include <string>
#include <iostream>


namespace abilities
{
	class AttackAbility
	{

	private:
		std::string m_name;
		int m_damage, m_manaCost;

	public:

		AttackAbility(std::string name = "NULL", int dmg = 0, int manaCost = 0);

		void execute();

	};


}
