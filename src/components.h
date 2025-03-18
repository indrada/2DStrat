#pragma once

#include <memory>

// C - component


class CStats
{

public:

	int m_hp, maxHp;

	int m_mana, maxMana;

	int m_level, m_exp;

	bool has = false;


	CStats(int hp = 100, int mana = 50, int level = 1)
		: m_hp(hp), m_mana(mana), m_level(level)
	{
		maxHp = m_hp;
		maxMana = m_mana;
		m_exp = 0;

		has = true;
	}
};

