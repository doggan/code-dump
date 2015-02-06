#include "NPCManager.h"

#include "ImageManager.h"
#include "ItemManager.h"
#include "Utilities.h"

#include "tinyxml.h"

void NPCManager::loadNPCs(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument(fileName);
	if (!doc->LoadFile())
		EXCEPTION(Exception::ERR_FILE_NOT_FOUND, "NPCManager::loadNPCs()", "Cannot load file: " + fileName);

	TiXmlElement *elem = doc->FirstChildElement();

	while (elem)
	{
		int id;
		elem->QueryIntAttribute("id", &id);
		
		NPC *npc = new NPC(id, elem->Attribute("name"));

		std::string comb;
		if (elem->QueryValueAttribute("combative", &comb) == TIXML_SUCCESS)
		{
			if (comb == "no")
				npc->setCombative(false);
			else
				npc->setCombative(true);
		}

		std::string pol;
		if (elem->QueryValueAttribute("polarity", &pol) == TIXML_SUCCESS)
		{
			if (pol == "light")
				npc->setPolarity(LIGHT);
			else
				npc->setPolarity(DARK);
		}

		// Read in each NPC details.
		TiXmlElement *child = elem->FirstChildElement();
		while (child)
		{
			if (child->ValueStr() == "texture")
			{
				TiXmlText *textureName = dynamic_cast<TiXmlText*>(child->FirstChild());
				if (textureName)
				{
					if (npc->getPolarity() == LIGHT)
						ImageManager::getSingleton().getImage(std::string(textureName->Value()), npc->m_imageLight);
					else
						ImageManager::getSingleton().getImage(std::string(textureName->Value()), npc->m_imageDark);
				}
			}
			else if (child->ValueStr() == "stats")
			{
				Stats s;
				child->QueryValueAttribute("level", &s.level);
				child->QueryValueAttribute("hp", &s.hp);
				
				if (child->QueryValueAttribute("energy", &s.energy) != TIXML_SUCCESS)
					s.energy = 0;

				npc->setStats(s);
			}
			else if (child->ValueStr() == "xp")
			{
				npc->setExp(strToInt(child->GetText()));
			}
			else if (child->ValueStr() == "loot")
			{
				TiXmlElement *lootElement = child->FirstChildElement();
				NPCData npcData;
				while (lootElement)
				{
					if (lootElement->ValueStr() == "item")
					{
						Loot l;
						lootElement->QueryValueAttribute("id", &l.id);
						lootElement->QueryValueAttribute("chance", &l.chance);
						npcData.lootVec.push_back(l);
					}
					else if (lootElement->ValueStr() == "gold")
					{
						lootElement->QueryValueAttribute("max", &npcData.goldMax);
					}

					lootElement = dynamic_cast<TiXmlElement*>(child->IterateChildren(dynamic_cast<TiXmlNode*>(lootElement)));
				}

				m_npcLootMap.insert(std::make_pair(npc->getID(), npcData));
			}

			child = dynamic_cast<TiXmlElement*>(elem->IterateChildren(dynamic_cast<TiXmlNode*>(child)));
		}

		addObject(npc);
		delete npc;

		elem = dynamic_cast<TiXmlElement*>(elem->NextSibling());
	}

	delete doc;
}

NPC *NPCManager::getObject(uint id)
{
	NPC *npc = Factory<NPC>::getObject(id);

	// Generate this NPC's loot.
	{
		NPCData npcData = m_npcLootMap[npc->getID()];

		// Gold.
		float gold = npcData.goldMax * rand01();
		npc->setGold(round(gold));

		// Items.
		for (uint i = 0; i < npcData.lootVec.size(); ++i)
		{
			float rnd = rand01();

			if (npcData.lootVec[i].chance != 0 && npcData.lootVec[i].chance >= rnd)
			{
				Item *item = ItemManager::getSingleton().getObject(npcData.lootVec[i].id);
				npc->addToInventory(item, true);
			}
		}
	}
	
	return npc;
}