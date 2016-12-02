#include "dataservice.h"
#include "XMLDocumentParser.h"
#include "config.h"

#include "areatemplate.h"
#include "itemtemplate.h"
#include "skilltemplate.h"
#include "abnormalitytemplate.h"
#include "passivitytemplate.h"
#include "equipmentsettemplate.h"
#include "customizingitemtemplate.h"
#include "passivitycategorytemplate.h"
#include "equipmentdatatemplate.h"
#include "enchantdatatemplate.h"
#include "npctemplate.h"

#include <sstream>
#include <iostream>

bool data_service::data_init()
{
	if (!data_load_abnormalities())
	{
		std::cout << "::FAILED TO LOAD ABNORMALITIES\n";
		return false;
	}

	//if (!data_load_passivities())
	//{
	//	std::cout << "::FAILED TO LOAD PASSIVITIES\n";
	//	return false;
	//}
	//
	//if (!data_load_equipment())
	//{
	//	std::cout << "::FAILED TO LOAD EQUIPMENT DATA\n";
	//	return false;
	//}

	if (!data_load_items())
	{
		std::cout << "::FAILED TO LOAD ITEMS\n";
		return false;
	}

	//std::string dir = config::dir.dataPlayerSkills; dir += "//AnimationData.xml";
	//if (!XMLDocumentParser::ParseAnimationDataXMLDocument(dir.c_str(), _animation_sets))
	//{
	//	std::cout << "::FAILED TO LOAD ANIMATIONS\n";
	//	return false;
	//}
	//
	//if (!data_load_skills())
	//{
	//	return false;
	//}

	data_equipment_final_build();
	return true;
}

void data_service::data_release()
{
	for (size_t i = 0; i < _abnormalities.size(); i++)
	{
		if (_abnormalities[i])
		{
			delete _abnormalities[i];
			_abnormalities[i] = 0;
		}
	}

	for (size_t i = 0; i < _passivities.size(); i++)
	{
		if (_passivities[i])
		{
			delete _passivities[i];
			_passivities[i] = 0;
		}
	}
	_passivities.clear();

	for (size_t i = 0; i < _passivity_categories.size(); i++)
	{
		if (_passivity_categories[i])
		{
			delete _passivity_categories[i];
			_passivity_categories[i] = nullptr;
		}
	}
	_passivity_categories.clear();


	for (size_t i = 0; i < _equipment_sets.size(); i++)
	{
		if (_equipment_sets[i])
		{
			delete _equipment_sets[i];
			_equipment_sets[i] = nullptr;
		}
	}
	_equipment_sets.clear();

	for (size_t i = 0; i < _customizing_items.size(); i++)
	{
		if (_customizing_items[i])
		{
			delete _customizing_items[i];
			_customizing_items[i] = nullptr;
		}
	}
	_customizing_items.clear();

	for (size_t i = 0; i < _equipment_data.size(); i++)
	{
		if (_equipment_data[i])
		{
			delete _equipment_data[i];
			_equipment_data[i] = nullptr;
		}
	}
	_equipment_data.clear();

	for (size_t i = 0; i < _items.size(); i++)
	{
		if (_items[i])
		{
			delete _items[i];
			_items[i] = 0;
		}
	}
	_items.clear();

	for (size_t i = 0; i < _animation_sets.size(); i++)
	{
		if (_animation_sets[i])
		{
			delete _animation_sets[i];
			_animation_sets[i] = nullptr;
		}
	}
	_animation_sets.clear();

#pragma region skills
	for (size_t i = 0; i < _archer.size(); i++)
	{
		if (_archer[i])
		{
			delete _archer[i];
			_archer[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _warrior.size(); i++)
	{
		if (_warrior[i])
		{
			delete _warrior[i];
			_warrior[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _berserker.size(); i++)
	{
		if (_berserker[i])
		{
			delete _berserker[i];
			_berserker[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _lancer.size(); i++)
	{
		if (_lancer[i])
		{
			delete _lancer[i];
			_lancer[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _slayer.size(); i++)
	{
		if (_slayer[i])
		{
			delete _slayer[i];
			_slayer[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _priest.size(); i++)
	{
		if (_priest[i])
		{
			delete _priest[i];
			_priest[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _mystic.size(); i++)
	{
		if (_mystic[i])
		{
			delete _mystic[i];
			_mystic[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _reaper.size(); i++)
	{
		if (_reaper[i])
		{
			delete _reaper[i];
			_reaper[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _gunner.size(); i++)
	{
		if (_gunner[i])
		{
			delete _gunner[i];
			_gunner[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _brawler.size(); i++)
	{
		if (_brawler[i])
		{
			delete _brawler[i];
			_brawler[i] = nullptr;
		}
	}

	for (size_t i = 0; i < _ninja.size(); i++)
	{
		if (_ninja[i])
		{
			delete _ninja[i];
			_ninja[i] = nullptr;
		}
	}
#pragma endregion
}

passivity_template* data_service::data_resolve_passivity(uint32 id)
{
	for (size_t i = 0; i < _passivities.size(); i++)
	{
		if (_passivities[i] && _passivities[i]->id == id)
			return (passivity_template*)_passivities[i];
	}
	return nullptr;
}

passivity_category* data_service::data_resolve_passivity_category(uint32 id)
{
	for (size_t i = 0; i < _passivity_categories.size(); i++)
	{
		if (_passivity_categories[i] && _passivity_categories[i]->id == id)
			return (passivity_category*)_passivity_categories[i];
	}
	return nullptr;
}

item_template* data_service::data_resolve_item(uint32 id)
{
	for (size_t i = 0; i < _items.size(); i++)
		if (_items[i] && _items[i]->id == id)
			return (item_template*)_items[i];
	

	return nullptr;
}

abnormality_template * data_service::data_resolve_abnormality(uint32 id)
{
	for (size_t i = 0; i < _abnormalities.size(); i++)
	{
		if (_abnormalities[i] && _abnormalities[i]->id == id)
			return (abnormality_template *)_abnormalities[i];
	}
	return nullptr;
}

skill_template * data_service::data_resolve_skill(uint32 id)
{
	return NULL;
}

enchant_set * data_service::data_resolve_enchant_set(uint32 id)
{
	for (size_t i = 0; i < _enchant_sets.size(); i++)
	{
		if (_enchant_sets[i] && _enchant_sets[i]->id == id)
			return (enchant_set *)_enchant_sets[i];
	}
	return nullptr;
}

equipment_data * data_service::data_resolve_equipment_data(uint32 id)
{
	for (size_t i = 0; i < _equipment_data.size(); i++)
	{
		if (_equipment_data[i] && _equipment_data[i]->id == id)
			return (equipment_data *)_equipment_data[i];
	}
	return nullptr;
}

cusotmizing_item * data_service::data_resolve_customizing_item(uint32 id)
{
	for (size_t i = 0; i < _customizing_items.size(); i++)
	{
		if (_customizing_items[i] && _customizing_items[i]->id == id)
			return (cusotmizing_item *)_customizing_items[i];
	}
	return nullptr;
}

Animation * data_service::data_resolve_animation(STRING animation_set_name, STRING animation_name)
{
	for (uint32 i = 0; i < _animation_sets.size(); i++)
	{
		if (_animation_sets[i] && strcmp(_animation_sets[i]->name.c_str(), animation_set_name))
		{
			for (uint32 j = 0; j < _animation_sets[i]->animations.size(); j++)
			{
				if (strcmp(_animation_sets[i]->animations[j].name.c_str(), animation_name))
					return (Animation *)&(_animation_sets[i]->animations[j]);
			}
		}
	}

	return nullptr;
}

uint32 data_service::data_get_skill_base_id(uint32 skill_id)
{
	skill_id /= 10000;
	skill_id *= 10000;
	skill_id += 100;
	return skill_id;
}



bool data_service::data_load_abnormalities()
{
	std::string line = config::dir.dataAbnormalities;
	line += "//info.txt";
	std::ifstream infoFile = std::ifstream(line);
	if (!infoFile.is_open())
		return false;
	int toReadCount = 0;
	line.clear();
	std::getline(infoFile, line);
	sscanf(line.c_str(), "count =%d", &toReadCount);
	infoFile.close();

	std::stringstream ss;

	for (size_t i = 0; i < toReadCount; i++)
	{
		ss.clear(); ss.str("");
		ss << config::dir.dataAbnormalities << "//Abnormality-" << i << ".xml";
		std::cout << "Loading abnormalities from [Abnormality-" << i << ".xml]\n";
		if (!XMLDocumentParser::ParseAbnormalityXMLDocument(ss.str().c_str(), _abnormalities))
			continue;
	}


	return true;
}

bool data_service::data_load_passivities()
{
	std::stringstream ss;
	ss << config::dir.dataPassivities << "\\info.txt" << '\0';
	std::fstream infoFile = std::fstream(ss.str());
	if (!infoFile.is_open())
		return false;

	int toReadFileCount = 0; std::string line;
	std::getline(infoFile, line); sscanf(line.c_str(), "count =%d", &toReadFileCount);
	if (toReadFileCount == 0)
		return false;

	for (size_t i = 0; i < toReadFileCount; i++)
	{
		ss.clear();
		ss.str("");
		ss << config::dir.dataPassivities << "//Passivity-" << i << ".xml" << '\0';
		std::cout << "Loading passivities from [Passivity-" << i << ".xml]\n";
		if (!XMLDocumentParser::ParsePassivityXMLDocument(ss.str().c_str(), _passivities))
			continue;

	}

	ss.clear();
	ss.str("");
	ss<< config::dir.dataPassivities << "\\PassivitiesCategories.xml" << '\0';
	if (!XMLDocumentParser::ParsePassivityCategoriesXMLDocument(ss.str().c_str(), _passivity_categories))
		return false;

	return true;
}

bool data_service::data_load_skills()
{
	std::string fileName;

	fileName = config::dir.dataPlayerSkills; fileName += "//warrior.xml";
	if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _warrior))
	{
		std::cout << "::FAILED TO LOAD WARRIOR SKILLS. [" << fileName << "]\n\n";
		return false;
	}

	//fileName = file; fileName += "//PlayerSkills//berserker.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _berserker))
	//{
	//	std::cout << "::FAILED TO LOAD BERSERKER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//lancer.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _lancer))
	//{
	//	std::cout << "::FAILED TO LOAD LANCER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//slayer.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _slayer))
	//{
	//	std::cout << "::FAILED TO LOAD SLAYER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//priest.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _priest))
	//{
	//	std::cout << "::FAILED TO LOAD PRIEST SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//mystic.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _mystic))
	//{
	//	std::cout << "::FAILED TO LOAD MYSTIC SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//reaper.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _reaper))
	//{
	//	std::cout << "::FAILED TO LOAD REAPER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//gunner.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _gunner))
	//{
	//	std::cout << "::FAILED TO LOAD GUNNER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//brawler.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _brawler))
	//{
	//	std::cout << "::FAILED TO LOAD BRAWLER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}
	//
	//fileName = file; fileName += "//PlayerSkills//assasin.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _ninja))
	//{
	//	std::cout << "::FAILED TO LOAD ASSASIN SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}

	//fileName = file; fileName += "//PlayerSkills//sorcerer.xml";
	//if (!XMLDocumentParser::ParseSkillXMLDocument(fileName.c_str(), _sorcerer))
	//{
	//	std::cout << "::FAILED TO LOAD SORCERER SKILLS. [" << fileName << "]\n\n";
	//	return false;
	//}

	return true;
}

bool data_service::data_load_items()
{
	std::stringstream ss; std::string line; int readType = 1;

	ss << config::dir.dataItems << "//info.txt";
	std::ifstream file = std::ifstream(ss.str());
	if (!file.is_open())
		return false;
	std::getline(file, line);
	int fileCount = 0;
	sscanf(line.c_str(), "Count = %d", &fileCount);



	file.close();
	uint32 itemCountNow = 0; uint32 itemCountOld = 0;
	for (size_t i = 0; i < fileCount; i++)
	{
		itemCountOld = (uint32)_items.size();
		ss.clear(); ss.str("");
		ss << config::dir.dataItems << "//ItemData-" << i << ".xml" << '\0';
		std::cout << ">Loading [ItemData-" << i << ".xml]...\n";
		if (!XMLDocumentParser::ParseItemsXMLDocument(ss.str().c_str(), _items))
			continue;
		itemCountNow = (uint32)_items.size();
	}

	if (!XMLDocumentParser::ParseItemsXMLDocument(".//data//Items//ItemData-test.xml", _items))
		return false;

	return true;
}

bool data_service::data_load_equipment()
{
	std::stringstream ss;

	//ss << config::dir.dataEquipment;
	//ss << "//EquipmentSetData.xml";
	//if (!data_load_equipment_sets(ss.str().c_str()))
	//	return false;
	//ss.str(""); ss.clear();

	//ss << config::dir.dataEquipment;
	//ss << "//CustomizingItems.xml";
	//if (!data_load_customizing_items(ss.str().c_str()))
	//	return false;
	//ss.str(""); ss.clear();

	ss << config::dir.dataEquipment;
	ss << "//EquipmentData-test.xml";
	if (!data_load_equipment_data(ss.str().c_str()))
		return false;
	ss.str(""); ss.clear();


	//ss << config::dir.dataEquipment;
	//ss << "//EquipmentEnchantData.xml";
	//if (!data_load_enchant_data(ss.str().c_str()))
	//	return false;
	//ss.str(""); ss.clear();

	return true;
}

bool data_service::data_load_equipment_sets(STRING file)
{
	if (!XMLDocumentParser::ParseEquipmentSetXMLDocument(file, _equipment_sets))
	{
		return false;
	}
	return true;
}

bool data_service::data_load_customizing_items(STRING file)
{
	if (!XMLDocumentParser::ParseCustomizingItmesXMLDocument(file, _customizing_items))
	{
		return false;
	}

	return true;
}

bool data_service::data_load_equipment_data(STRING file)
{
	if (!XMLDocumentParser::ParseEquipmentDataXMLDocument(file, _equipment_data))
	{
		return false;
	}
	return true;
}

bool data_service::data_load_enchant_data(STRING file)
{
	if (!XMLDocumentParser::ParseEnchantDataXMLDocument(file, _enchant_sets))
	{
		return false;
	}
	return true;
}

void data_service::data_equipment_final_build()
{
	for (size_t i = 0; i < _equipment_sets.size(); i++)
	{
		if (_equipment_sets[i])
		{
			for (size_t k = 0; k < _equipment_sets[i]->items.size(); k++)
			{
				for (size_t j = 0; j < _equipment_sets[i]->items[k].ids->size(); j++)
				{
					item_template* toInsert = data_resolve_item((*_equipment_sets[i]->items[k].ids)[j]);
					if (toInsert)
					{
						toInsert->equipmentSetData = (equipment_set_data*)_equipment_sets[k];
						((equipment_set*)_equipment_sets[i])->items[k].items.push_back(toInsert);
					}
				}

				_equipment_sets[i]->items[k].ids->clear();
				delete _equipment_sets[i]->items[k].ids;
				((equipment_set*)_equipment_sets[i])->items[k].ids = nullptr;

			}
		}
	}
}

std::vector<const area_template *>			data_service::_areas;
std::vector<const passivity_template *>		data_service::_passivities;
std::vector<const abnormality_template *>	data_service::_abnormalities;
std::vector<const passivity_category *>		data_service::_passivity_categories;
std::vector<const npc_template * >			data_service::_npcs;
std::vector<const animation_set *>			data_service::_animations;
std::vector<const equipment_set_data *>		data_service::_equipment_sets;
std::vector<const equipment_data *>			data_service::_equipment_data;
std::vector<const enchant_set * >			data_service::_enchant_sets;
std::vector<const cusotmizing_item *>		data_service::_customizing_items;
std::vector<const item_template*>			data_service::_items;
std::vector<const animation_set*>			data_service::_animation_sets;


std::vector<const skill_template *>			data_service::_warrior;
std::vector<const skill_template *>			data_service::_archer;
std::vector<const skill_template *>			data_service::_berserker;
std::vector<const skill_template *>			data_service::_slayer;
std::vector<const skill_template *>			data_service::_sorcerer;
std::vector<const skill_template *>			data_service::_lancer;
std::vector<const skill_template *>			data_service::_priest;
std::vector<const skill_template *>			data_service::_mystic;
std::vector<const skill_template *>			data_service::_reaper;
std::vector<const skill_template *>			data_service::_gunner;
std::vector<const skill_template *>			data_service::_brawler;
std::vector<const skill_template *>			data_service::_ninja;