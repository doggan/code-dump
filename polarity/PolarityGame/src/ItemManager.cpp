#include "ItemManager.h"

#include "Item.h"
#include "Utilities.h"

#include "tinyxml.h"

void ItemManager::loadItems(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument(fileName);
	if (!doc->LoadFile())
		EXCEPTION(Exception::ERR_FILE_NOT_FOUND, "ItemManager::loadItems()", "Cannot load file: " + fileName);

	TiXmlElement *elem = doc->FirstChildElement();

	while (elem)
	{
		int id;
		elem->QueryIntAttribute("id", &id);
	
		std::string name = elem->Attribute("name");

		Item *item = new Item(id, name);

		Stats stats;

		// Read in each item details.
		TiXmlElement *child = elem->FirstChildElement();
		while (child)
		{
			if (child->ValueStr() == "slot")
			{
				std::string slotStr = child->GetText();
				Item::SLOT slot;

				if (slotStr == "weapon")
					slot = Item::WEAPON;
				else if (slotStr == "chest")
					slot = Item::CHEST;
				else if (slotStr == "head")
					slot = Item::HEAD;
				else
					slot = Item::GENERAL;

				item->setSlot(slot);
			}
			else if (child->ValueStr() == "hp")
			{
				stats.hp = strToInt(child->GetText());
			}
			else if (child->ValueStr() == "energy")
			{
				stats.energy = strToInt(child->GetText());
			}
			else if (child->ValueStr() == "str")
			{
				stats.str = strToInt(child->GetText());
			}
			else if (child->ValueStr() == "int")
			{
				stats.intel = strToInt(child->GetText());
			}

			child = dynamic_cast<TiXmlElement*>(elem->IterateChildren(dynamic_cast<TiXmlNode*>(child)));
		}

		item->setStats(stats);

		addObject(item);
		delete item;

		elem = dynamic_cast<TiXmlElement*>(elem->NextSibling());
	}

	delete doc;
}