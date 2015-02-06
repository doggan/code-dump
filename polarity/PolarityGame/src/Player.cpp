#include "Player.h"

#include "GUI.h"
#include "ImageManager.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemManager.h"
#include "Utilities.h"

#include "tinyxml.h"

Player::Player(const std::string &name)
: EntityBase(0, name)
{
	m_collisionBox.w = 29;
	m_collisionBox.h = 20;
	m_collisionBox.x = 0;
	m_collisionBox.y = 14;

	m_targetBox.w = 29;
	m_targetBox.h = 35;
	m_targetBox.x = 0;
	m_targetBox.y = 0;

	// TODO:
	m_pos.x = m_pos.y = 60;

	m_lEnergy = m_dEnergy = 0;

	ImageManager::getSingleton().getImage("player_dark.png", m_imageDark);
	ImageManager::getSingleton().getImage("player_light.png", m_imageLight);

	m_inventory = new Inventory(this);
}

void Player::load(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument(fileName);
	if (!doc->LoadFile())
		EXCEPTION(Exception::ERR_FILE_NOT_FOUND, "Player::load()", "Cannot load file: " + fileName);

	TiXmlElement *elem = doc->FirstChildElement();

	while (elem)
	{
		m_name = elem->Attribute("name");

		// Read in Player details.
		TiXmlElement *child = elem->FirstChildElement();
		while (child)
		{
			if (child->ValueStr() == "stats")
			{
				child->QueryValueAttribute("level", &m_stats.level);
				child->QueryValueAttribute("hp", &m_stats.hp);
				child->QueryValueAttribute("lenergy", &m_lEnergy);
				child->QueryValueAttribute("denergy", &m_dEnergy);
				m_stats.energy = m_lEnergy + m_dEnergy;
			}
			else if (child->ValueStr() == "speed")
			{
				m_movementSpeed = strToFloat(child->GetText());
			}
			else if (child->ValueStr() == "inventory")
			{
				TiXmlElement *lootElement = child->FirstChildElement();
				while (lootElement)
				{
					if (lootElement->ValueStr() == "item")
					{
						uint id;
						lootElement->QueryValueAttribute("id", &id);

						std::string isEquipped;
						lootElement->QueryValueAttribute("equipped", &isEquipped);

						bool equip = false;
						if (isEquipped == "yes")
							equip = true;

						addToInventory(ItemManager::getSingleton().getObject(id), equip);
					}

					lootElement = dynamic_cast<TiXmlElement*>(child->IterateChildren(dynamic_cast<TiXmlNode*>(lootElement)));
				}
			}

			child = dynamic_cast<TiXmlElement*>(elem->IterateChildren(dynamic_cast<TiXmlNode*>(child)));
		}

		elem = dynamic_cast<TiXmlElement*>(elem->NextSibling());
	}

	delete doc;
}

void Player::save(const std::string &fileName)
{	
	TiXmlDocument doc;

	TiXmlElement *player = new TiXmlElement("player");
	player->SetAttribute("name", m_name);
	doc.LinkEndChild(player);

	// Stats.
	{
		// Temporarily de-equip all player items (so we can save base-stats)...
		std::set< Item* > itemSet = m_inventory->unequipAll();

		TiXmlElement *stats = new TiXmlElement("stats");
		stats->SetAttribute("level", m_stats.level);
		stats->SetAttribute("hp", m_stats.hp);
		stats->SetAttribute("lenergy", m_lEnergy);
		stats->SetAttribute("denergy", m_dEnergy);
		player->LinkEndChild(stats);

		TiXmlElement *speed = new TiXmlElement("speed");
		speed->LinkEndChild(new TiXmlText(toStr(m_movementSpeed)));
		player->LinkEndChild(speed);

		// ...and re-equip.
		m_inventory->equip(itemSet);
	}

	// Inventory.
	{
		TiXmlElement *inv = new TiXmlElement("inventory");

		// Inventory items.
		std::set< Item* > itemSet = m_inventory->getInventoryItems();
		std::set< Item* >::const_iterator iter = itemSet.begin();
		for (iter; iter != itemSet.end(); ++iter)
		{
			TiXmlElement *item = new TiXmlElement("item");
			item->SetAttribute("id", (*iter)->getID());
			item->SetAttribute("equipped", "no");
			inv->LinkEndChild(item);
		}

		// Equipped items.
		itemSet = m_inventory->getEquippedItems();
		iter = itemSet.begin();
		for (iter; iter != itemSet.end(); ++iter)
		{
			TiXmlElement *item = new TiXmlElement("item");
			item->SetAttribute("id", (*iter)->getID());
			item->SetAttribute("equipped", "yes");
			inv->LinkEndChild(item);
		}

		player->LinkEndChild(inv);
	}

	doc.SaveFile(fileName);
}

void Player::update(float deltaTime)
{
	EntityBase::update(deltaTime);

	GUI::getSingleton().setPlayerInfo(m_stats, m_xp, m_lEnergy, m_dEnergy);
	GUI::getSingleton().setInventory(m_inventory, m_stats);
}

void Player::kill()
{
	EntityBase::kill();

	GUI::getSingleton().addToChat("You have been slain by " + m_recentAffector->getName() + "!");
}