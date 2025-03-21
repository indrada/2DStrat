#pragma once

#include <memory>

// C - component


struct CStats
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

enum class ELEMENT{FIRE, WATER, EARTH, AIR, NONE};

struct CBattleStats
{

public:

	int m_damage, m_defence;

	ELEMENT m_elemnt;

	bool has = false;

	CBattleStats(int damage = 0, int defence = 0, ELEMENT elem = ELEMENT::NONE)
		:m_damage(damage), m_defence(defence), m_elemnt(elem)
	{
		has = true;
	}
};