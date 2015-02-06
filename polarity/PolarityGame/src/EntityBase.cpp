#include "EntityBase.h"

#include "Application.h"
#include "Inventory.h"
#include "ProjectileManager.h"
#include "GUI.h"

EntityBase::EntityBase(uint id, const std::string &name)
{
	m_id = id;
	m_name = name;

	m_gold = 0;
	m_xp = 0;
	m_combative = true;

	m_currentTarget = NULL;
	m_polarity = DARK;

	m_pos.x = m_pos.y = 0;
	m_direction = 0;
	m_movementSpeed = 100;

	m_imageDark = NULL;
	m_imageLight = NULL;

	m_inventory = NULL;

	m_alive = true;
	m_recentAffector = NULL;
}

EntityBase::EntityBase(const EntityBase &eb)
{
	m_name = eb.m_name;
	m_id = eb.m_id;

	m_gold = eb.m_gold;
	m_xp = eb.m_xp;
	m_combative = eb.m_combative;

	m_alive = eb.m_alive;
	m_recentAffector = eb.m_recentAffector;

	m_collisionBox = eb.m_collisionBox;
	m_targetBox = eb.m_targetBox;
	m_polarity = eb.m_polarity;

	m_pos = eb.m_pos;
	m_direction = eb.m_direction;
	m_movementSpeed = eb.m_movementSpeed;

	m_currentTarget = eb.m_currentTarget;
	m_stats = eb.m_stats;

	m_spellList = eb.m_spellList;

	m_imageDark = eb.m_imageDark;
	m_imageLight = eb.m_imageLight;

	m_inventory = new Inventory(this);
}

EntityBase &EntityBase::operator=(const EntityBase &eb)
{
	// This function is necessary if the operation is performed.
	// No need for it yet, so not writing the function :P
	assert(!"Not yet implemented!");

	return *this;
}

EntityBase::~EntityBase()
{
	delete m_inventory;
}

void EntityBase::applyStats(const Stats &stats)
{
	m_stats = m_stats + stats;
}

void EntityBase::deapplyStats(const Stats &stats)
{
	m_stats = m_stats - stats;
}

void EntityBase::addToInventory(Item *i, bool autoEquip)
{
	m_inventory->addItem(i);

	if (autoEquip)
		m_inventory->equip(i);
}

void EntityBase::update(float deltaTime)
{
	if (m_combative && m_alive && m_stats.hp <= 0)
	{
		assert(m_recentAffector);
		kill();
	}
}

void EntityBase::kill()
{
	m_alive = false;
}