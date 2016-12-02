#include "XMLDocumentParser.h"
#include "servertime.h"
#include <iostream>
#include <string>

#include "playerEnums.h"

#include "dataservice.h"

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

const bool XMLDocumentParser::ParseAreaXMLDocument(const char* fileName, std::vector< const area_template *>& out_areas)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		//std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into Areas...\n";
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
	}



	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (i == 1)
			BuildAreas(_mainNodes[i], out_areas);

		if (_mainNodes[i])
		{
			delete _mainNodes[i];
			_mainNodes[i] = 0;
		}
	}
	_mainNodes.clear();
	return true;
}
const bool XMLDocumentParser::ParseItemsXMLDocument(const char* fileName, std::vector<const item_template *>& out_items)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into Items...\n";
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
	}



	InitItemsDictionary();
	for (size_t i = 0; i < _mainNodes.size(); i++)
	{

		if (i == 1)
		{
			BuildItems(_mainNodes[i], out_items);
		}

		if (_mainNodes[i])
		{
			delete _mainNodes[i];
			_mainNodes[i] = 0;
		}
	}
	std::cout << "\n\n";
	_mainNodes.clear();
	return true;

}
const bool XMLDocumentParser::ParseSkillXMLDocument(const char * fileName, std::vector<const skill_template *>& out_skills)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into Skills...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
	}
	InitSkillsDictionary();


	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i])
		{
			if (_mainNodes[i]->tagName == "SkillData")
			{
				BuildSkill(_mainNodes[i], out_skills);
			}

			delete _mainNodes[i];
			_mainNodes[i] = nullptr;
		}
	}
	std::cout << "\n\n";
	_mainNodes.clear();



	return true;
}
const bool XMLDocumentParser::ParseAbnormalityXMLDocument(const char * fileName, std::vector<const abnormality_template *>& out_abnormalities)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into Abnormalities...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
	}


	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i])
		{
			if (_mainNodes[i]->tagName == "Abnormality")
			{
				BuildAbnormality(_mainNodes[i], out_abnormalities);
			}

			delete _mainNodes[i];
			_mainNodes[i] = 0;
		}
	}
	std::cout << "\n\n";
	_mainNodes.clear();
	return true;
}
const bool XMLDocumentParser::ParsePassivityXMLDocument(const char * fileName, std::vector<const passivity_template *>& out_passivities)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into Passivities...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
	}
	InitPassivityDictionary();



	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i])
		{
			if (_mainNodes[i]->tagName == "Passivity")
			{
				BuildPassivity(_mainNodes[i], out_passivities);
			}

			delete _mainNodes[i];
			_mainNodes[i] = 0;
		}
	}

	std::cout << "\n\n";
	_mainNodes.clear();
	return true;
}
const bool XMLDocumentParser::ParseEquipmentSetXMLDocument(const char * fileName, std::vector< const equipment_set_data *>& out_equipmentSets)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into EquipmentSetData...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}

	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i])
		{
			if (_mainNodes[i]->tagName == "EquipmentSetData")
			{
				for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
				{
					if (_mainNodes[i]->childNodes[j])
						BuildEquipmentSetData(_mainNodes[i]->childNodes[j], out_equipmentSets);
				}
			}

			delete _mainNodes[i];
			_mainNodes[i] = nullptr;
		}

	}

	std::cout << "::Parsed <<[" << out_equipmentSets.size() << "] equipmentSets\n\n";


	return true;
}
const bool XMLDocumentParser::ParseCustomizingItmesXMLDocument(const char * fileName, std::vector<const cusotmizing_item *>& out_costomizations)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into CustomizingItem...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}


	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i])
		{
			if (_mainNodes[i]->tagName == "CustomizingItems")
			{
				for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
				{
					if (_mainNodes[i]->childNodes[j])
						BuildCustomizingItem(_mainNodes[i]->childNodes[j], out_costomizations);
				}
			}

			delete _mainNodes[i];
			_mainNodes[i] = nullptr;
		}

	}
	std::cout << "::Parsed <<[" << out_costomizations.size() << "] customizingItems\n\n";

	return true;
}
const bool XMLDocumentParser::ParsePassivityCategoriesXMLDocument(const char * fileName, std::vector< const passivity_category *>& out_categories)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into PassivityCategories...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}

	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i] && _mainNodes[i]->tagName == "PassivityCategoryData")
		{
			for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
			{
				if (_mainNodes[i]->childNodes[j])
				{
					BuildPassivityCategory(_mainNodes[i]->childNodes[j], out_categories);
				}
			}

		}

		delete _mainNodes[i];
		_mainNodes[i] = nullptr;
	}
	std::cout << ">Parsed [" << out_categories.size() << "] passivityCategories.\n\n";

	return true;
}
const bool XMLDocumentParser::ParseEquipmentDataXMLDocument(const char * fileName, std::vector< const equipment_data *>& out_data)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into EquipmentData...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}

	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i] && _mainNodes[i]->tagName == "EquipmentData")
		{
			for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
			{
				if (_mainNodes[i]->childNodes[j])
				{
					BuildEquipmentData(_mainNodes[i]->childNodes[j], out_data);
				}
				std::cout << "::Parsed <<[" << out_data.size() << "] equipmentData.\r" << std::flush;
			}

		}

		delete _mainNodes[i];
		_mainNodes[i] = nullptr;
	}

	std::cout << "\n\n";
	return true;
}
const bool XMLDocumentParser::ParseEnchantDataXMLDocument(const char * fileName, std::vector< const enchant_set *>& out_data)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into EnchantData...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}

	basicStatEnchatKindDictionary.insert(std::pair<std::string, e_basic_stat_enchant_kind>("attack", attack));
	basicStatEnchatKindDictionary.insert(std::pair<std::string, e_basic_stat_enchant_kind>("defence", defence));
	basicStatEnchatKindDictionary.insert(std::pair<std::string, e_basic_stat_enchant_kind>("impact", impact));
	basicStatEnchatKindDictionary.insert(std::pair<std::string, e_basic_stat_enchant_kind>("balance", balance));

	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i] && _mainNodes[i]->tagName == "EquipmentEnchantData")
		{
			for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
			{
				for (size_t k = 0; k < _mainNodes[i]->childNodes[j]->childNodes.size(); k++)
				{
					if (_mainNodes[i]->childNodes[j])
						BuildEnchantData(_mainNodes[i]->childNodes[j], out_data);

				}
			}

		}

		delete _mainNodes[i];
		_mainNodes[i] = nullptr;
	}

	std::cout << ">Parsed [" << out_data.size() << "] enchatData.\n\n";
	return true;
}
const bool XMLDocumentParser::ParseRegionStrSheetXMLDocument(const char * fileName)
{
	std::vector<XMLNode*> out_nodes;
	ParseXMLDocument(out_nodes, fileName);

	for (size_t i = 0; i < out_nodes[1]->childNodes.size(); i++)
	{
		BuildRegionName(out_nodes[1]->childNodes[i]);
	}

	for (size_t i = 0; i < out_nodes.size(); i++)
	{
		delete out_nodes[i];
		out_nodes[i] = 0;
	}


	return true;
}
const bool XMLDocumentParser::ParseNpcTemplateXMLDocuemnt(const char * fileName, std::vector<const npc_template *>& out_data)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() < 2)
		{

			return false;
		}
	}
	else if (result == 1)
	{

		return false;
	}
	else if (result == 2)
	{

		return false;
	}
	XMLNodeArgument * arg = nullptr;
	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i] && _mainNodes[i]->tagName == "NpcData")
		{
			int huntingZoneId = 0;
			if ((arg = _mainNodes[i]->ConsumeArgument("huntingZoneId")))
				huntingZoneId = atoi(arg->argumentValue.c_str());

			for (size_t j = 0; j < _mainNodes[i]->childNodes.size(); j++)
			{
				if (_mainNodes[i]->childNodes[j])
					BuildNpcTemplate(_mainNodes[i]->childNodes[j], huntingZoneId, out_data);
			}

		}

		delete _mainNodes[i];
		_mainNodes[i] = nullptr;
	}

	return true;
}
const bool XMLDocumentParser::ParseAnimationDataXMLDocument(const char * fileName, std::vector<const animation_set *>& out_data)
{
	std::vector<XMLNode*> _mainNodes;
	byte result = ParseXMLDocument(_mainNodes, fileName);
	if (result == 0)
	{
		if (_mainNodes.size() >= 2)
			std::cout << ">Read [" << _mainNodes[1]->childNodes.size() - 1 << "] XMLNodes\n>Parsing XMLNodes into AnimationSets...\n";
		else
		{
			std::cout << "::ERROR [" << fileName << "] UNKNOWN FORMAT\n";
			return false;
		}
	}
	else if (result == 1)
	{
		std::cout << "::Failed to open XMLDocument [File] [" << fileName << "]\n";
		return false;
	}
	else if (result == 2)
	{
		std::cout << "::Nothing to read from [" << fileName << "]\n";
		return false;
	}

	for (size_t i = 0; i < _mainNodes.size(); i++)
	{
		if (_mainNodes[i] && _mainNodes[i]->tagName == "AnimationData")
			BuildAnimationSets(_mainNodes[i], out_data);

		delete _mainNodes[i];
		_mainNodes[i] = nullptr;
	}

	std::cout << ">Parsed [" << out_data.size() << "] animationSets.\n\n";
	return true;
}

const byte XMLDocumentParser::ParseXMLDocument(std::vector<XMLNode*>& out_nodes, const char * fileName)
{
	std::ifstream file = std::ifstream(fileName, std::ifstream::ate);
	if (!file.is_open())
		return 1;
	uint32 fileSize = (uint32)file.tellg();
	file.seekg(std::ifstream::beg);

	char * buffer = new char[fileSize];
	file.read(buffer, fileSize);
	file.close();
	if (fileSize <= 80)
	{
		return 2; // nothing to read
	}

	XMLNode * newNode = nullptr;
	int mainCursor = 0;
	for (size_t i = 0; i < 2; i++)
	{
		newNode = new XMLNode();
		mainCursor += GetNode(buffer, mainCursor, newNode);
		if (newNode)
		{
			out_nodes.push_back(newNode);
		}
	}


	delete[] buffer;
	buffer = 0;
	return 0;
}
int XMLDocumentParser::GetNode(char* buffer, int mainCursor, XMLNode* outNode)
{
	int argumentCursor = 0, valueCursor = 0, nameCursor = 0; byte result = 0;
	short argumentNameSize = 0, argumentValueSize = 0, nameSize;
	std::string name = "";


#pragma region NAME
	while (1)
	{
		if (buffer[mainCursor] == '<')
		{
			mainCursor++;

			while (buffer[mainCursor] == ' ')
				mainCursor++; //remove whiteSpaces

			nameCursor = mainCursor;

			while (buffer[nameCursor] != ' ' && buffer[nameCursor] != '>')
				nameCursor++;
			nameSize = nameCursor - mainCursor;
			name = "";
			if (nameSize <= 0)
			{
#ifdef XML_DEBUG
				result = 3;
				std::cout << "::NAME SIZE IS [" << nameSize << "] LINE[" << __LINE__ << "] ->ERROR\n\n";
#endif
				break;
			}
			for (size_t i = 0; i < nameSize; i++)
			{
				name += buffer[mainCursor + i];
			}
			break;
		}
		mainCursor++;
	}

	mainCursor += nameSize;
#pragma endregion
	outNode->tagName = name;

	while (buffer[mainCursor] == ' ')
		mainCursor++; //remove whiteSpaces

	if (name[0] == '/')
		return mainCursor;

	//case <tagname> or <tagname/>
	if (buffer[mainCursor] == '/' && buffer[mainCursor + 1] == '>')
	{
		mainCursor += 2; //go past />
		return mainCursor;
	}
	else if (buffer[mainCursor] == '>')
	{
		XMLNode* childNode = nullptr;
		while (1)
		{
			childNode = new XMLNode();
			childNode->parentNode = outNode;
			mainCursor = GetNode(buffer, mainCursor, childNode);
			outNode->childNodes.push_back(childNode);
			std::string toCheckName = name;
			toCheckName.insert(toCheckName.begin(), '/');
			if (toCheckName == childNode->tagName)
			{
				break;
			}
		}
		return mainCursor;
	}

#pragma region ARGUMENTS
	while (1)
	{
		while (buffer[mainCursor] == ' ')
			mainCursor++; //remove whiteSpaces

		if (buffer[mainCursor] == '>') // node with no arguments
		{
			argumentCursor++; // go past >
			XMLNode* childNode = nullptr;
			while (1)
			{
				childNode = new XMLNode();
				childNode->parentNode = outNode;
				argumentCursor = GetNode(buffer, argumentCursor, childNode);
				outNode->childNodes.push_back(childNode);
				std::string toCheckName = name;
				toCheckName.insert(toCheckName.begin(), '/');
				if (toCheckName == childNode->tagName)
				{
					break;
				}
			}

			mainCursor = argumentCursor;
			break;
		}
		else if (buffer[mainCursor] == '/' && buffer[mainCursor] == '>')
		{
			break;// no arguemtns , node finished
		}

		while (buffer[mainCursor] == ' ')
			mainCursor++; //remove whiteSpaces

		XMLNodeArgument * newArg = new XMLNodeArgument();
		valueCursor = 0; argumentCursor = mainCursor;


		while (1)
		{
			if (buffer[argumentCursor] == '\"' && valueCursor == 0)
			{
				if (buffer[argumentCursor + 1] == '\"') //case argName=""
				{
					argumentCursor++;
					valueCursor = argumentCursor;
					argumentCursor++;
					break;
				}

				argumentCursor++;
				valueCursor = argumentCursor;

			}
			else if (buffer[argumentCursor] == '\"' && valueCursor != 0)
			{
				argumentCursor++;
				break;
			}

			argumentCursor++;
		}

		argumentValueSize = argumentCursor - valueCursor - 1;
		argumentNameSize = valueCursor - mainCursor - 2;
		for (size_t i = 0; i < argumentNameSize; i++)
		{
			newArg->argumentName += buffer[mainCursor + i];
		}
		for (size_t i = 0; i < argumentValueSize; i++)
		{
			newArg->argumentValue += buffer[valueCursor + i];
		}
		outNode->_arguments.push_back(newArg);

		while (buffer[argumentCursor] == ' ')
			argumentCursor++;

		if (buffer[argumentCursor] == '>')
		{
			argumentCursor++; // go past >
			XMLNode* childNode = nullptr;
			while (1)
			{
				childNode = new XMLNode();
				childNode->parentNode = outNode;
				argumentCursor = GetNode(buffer, argumentCursor, childNode);
				outNode->childNodes.push_back(childNode);
				std::string toCheckName = name;
				toCheckName.insert(toCheckName.begin(), '/');
				if (toCheckName == childNode->tagName)
				{
					break;
				}
			}

			mainCursor = argumentCursor;
			break;
		}
		else if (buffer[argumentCursor] == '/' && buffer[argumentCursor + 1] == '>')
		{
			//we dont have ChildNodes return size
			argumentCursor += 2; //go past />
			mainCursor = argumentCursor;
			return mainCursor;
		}
		else if (buffer[argumentCursor] == '?' && buffer[argumentCursor + 1] == '>')
		{
			//we dont have ChildNodes return size
			argumentCursor += 2; //go past />
			mainCursor = argumentCursor;
			return mainCursor;
		}
		mainCursor += argumentNameSize + argumentValueSize + 3;
	}
#pragma endregion
	return mainCursor;
}

void XMLDocumentParser::BuildRegionName(XMLNode * node)
{
	XMLNodeArgument  *arg = nullptr;
	int id = 0; std::string name = "";
	for (size_t i = 0; i < node->_arguments.size(); i++)
	{
		arg = node->ConsumeArgument("id");
		if (arg)
			id = atoi(arg->argumentValue.c_str());
		else
			continue;

		arg = node->ConsumeArgument("string");
		if (arg)
			name = arg->argumentValue;
		else
			continue;

		regionsNames.insert(std::pair<int, std::string>(id, name));
	}
}
void XMLDocumentParser::BuildAreas(XMLNode* mainNode, std::vector<const area_template *>& out_areas)
{
	XMLNodeArgument* arg = nullptr;
	lparea_template area = new area_template;
	area_section newSection; 
	area_section subSection;
	XMLNode* subNode = nullptr;

	for (size_t i = 0; i < mainNode->_arguments.size(); i++)
	{
		if ((arg = mainNode->ConsumeArgument("continentId")))
		{
			area->continentId = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = mainNode->ConsumeArgument("nameId")))
		{
			area->name = regionsNames[atoi(arg->argumentValue.c_str())];
		}
		else if ((arg = mainNode->ConsumeArgument("ride")))
		{
			area->ride = arg->argumentValue == "True" ? true : false;
		}
		else if ((arg = mainNode->ConsumeArgument("worldMapGuardId")))
		{
			area->worldMapGuardId = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = mainNode->ConsumeArgument("worldMapWorldId")))
		{
			area->worldMapWorldId = atoi(arg->argumentValue.c_str());
		}
	}

	for (size_t i = 0; i < mainNode->childNodes.size(); i++)
	{
		if (!mainNode->childNodes[i] || mainNode->childNodes[i]->tagName != "Section")
			continue;
		subNode = mainNode->childNodes[i];
		memset(&newSection, 0, sizeof(area_section));

		for (size_t j = 0; j < subNode->_arguments.size(); j++)
		{
			if ((arg = subNode->ConsumeArgument("huntingZoneId")))
			{
				newSection.huntingZoneId = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("priority")))
			{
				newSection.priority = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("ignoreObstacleShortTel")))
			{
				newSection.ignoreObstacleShortTel = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("pk")))
			{
				newSection.pk = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("ride")))
			{
				newSection.ride = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("vender")))
			{
				newSection.vender = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("addMaxZ")))
			{
				newSection.addMaxZ = (float)atof(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("subtractMinZ")))
			{
				newSection.subtractMinZ = (float)atof(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("campId")))
			{
				newSection.campId = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("floor")))
			{
				newSection.floor = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("restBonus")))
			{
				newSection.restExpBonus = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("worldMapSectionId")))
			{
				newSection.worldMapSectionId = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("nameId")))
			{
				newSection.nameId = atoi(arg->argumentValue.c_str());
				newSection.name = regionsNames[atoi(arg->argumentValue.c_str())];
			}

		}

		XMLNode* subSubNode = nullptr;
		for (size_t i = 0; i < subNode->childNodes.size(); i++)
		{
			subSubNode = subNode->childNodes[i];
			if (subSubNode && subSubNode->tagName == "Fence")
			{
				for (size_t i = 0; i < subSubNode->_arguments.size(); i++)
				{
					if ((arg = subSubNode->ConsumeArgument("pos")))
					{
						area_fence newFence;
						memset(&newFence, 0, sizeof area_fence);
						sscanf(arg->argumentValue.c_str(), "%f,%f,%f", &newFence.x, &newFence.y, &newFence.z);
						newSection.fences.push_back(std::move(newFence));
					}
				}
			}
			else if (subSubNode && subSubNode->tagName == "Section")
			{
				memset(&subSection, 0, sizeof area_section);
				for (size_t j = 0; j < subSubNode->_arguments.size(); j++)
				{
					if ((arg = subSubNode->ConsumeArgument("huntingZoneId")))
					{
						subSection.huntingZoneId = atoi(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("priority")))
					{
						subSection.priority = atoi(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("ignoreObstacleShortTel")))
					{
						subSection.ignoreObstacleShortTel = arg->argumentValue == "True" ? true : false;
					}
					else if ((arg = subSubNode->ConsumeArgument("pk")))
					{
						subSection.pk = arg->argumentValue == "True" ? true : false;
					}
					else if ((arg = subSubNode->ConsumeArgument("ride")))
					{
						subSection.ride = arg->argumentValue == "True" ? true : false;
					}
					else if ((arg = subSubNode->ConsumeArgument("vender")))
					{
						subSection.vender = arg->argumentValue == "True" ? true : false;
					}
					else if ((arg = subSubNode->ConsumeArgument("addMaxZ")))
					{
						subSection.addMaxZ = (float)atof(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("subtractMinZ")))
					{
						subSection.subtractMinZ = (float)atof(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("campId")))
					{
						subSection.campId = atoi(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("floor")))
					{
						subSection.floor = atoi(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("restBonus")))
					{
						subSection.restExpBonus = arg->argumentValue == "True" ? true : false;
					}
					else if ((arg = subSubNode->ConsumeArgument("worldMapSectionId")))
					{
						subSection.worldMapSectionId = atoi(arg->argumentValue.c_str());
					}
					else if ((arg = subSubNode->ConsumeArgument("nameId")))
					{
						subSection.nameId = atoi(arg->argumentValue.c_str());
						subSection.name = regionsNames[atoi(arg->argumentValue.c_str())];
					}

				}
				area->_sections.push_back(std::move(subSection));
			}
		}
		area->_sections.push_back(newSection);
	}
	/*for (size_t i = 0; i < subNode->childNodes.size(); i++)
	{
		if (!subNode->childNodes[i] || subNode->childNodes[i]->tagName != "Section")
			continue;

		subNode = subNode->childNodes[i];
		newSection = new AreaSection();
		memset(newSection, 0, sizeof(AreaSection));

		for (size_t j = 0; j < subNode->_arguments.size(); j++)
		{
			if ((arg = subNode->ConsumeArgument("huntingZoneId")))
			{
				newSection.huntingZoneId = atoi(arg->argumentName.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("priority")))
			{
				newSection.priority = atoi(arg->argumentName.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("ignoreObstacleShortTel")))
			{
				newSection.ignoreObstacleShortTel = arg->argumentName == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("pk")))
			{
				newSection.pk = arg->argumentName == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("ride")))
			{
				newSection.ride = arg->argumentName == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("vender")))
			{
				newSection.vender = arg->argumentName == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("addMaxZ")))
			{
				newSection.addMaxZ = (float)atof(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("subtractMinZ")))
			{
				newSection.subtractMinZ = (float)atof(arg->argumentValue.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("campId")))
			{
				newSection.campId = atoi(arg->argumentName.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("floor")))
			{
				newSection.floor = atoi(arg->argumentName.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("restBonus")))
			{
				newSection.restExpBonus = arg->argumentName == "True" ? true : false;
			}
			else if ((arg = subNode->ConsumeArgument("worldMapSectionId")))
			{
				newSection.worldMapSectionId = atoi(arg->argumentName.c_str());
			}
			else if ((arg = subNode->ConsumeArgument("nameId")))
			{
				newSection.name = regionsNames[atoi(arg->argumentValue.c_str())];
			}


		}
		area->_sections.push_back(newSection);
	}
	}
*/

	out_areas.push_back(area);
}
void XMLDocumentParser::BuildItems(XMLNode * mainnode, std::vector<const item_template *>& out_items)
{
	int parsedPerSecond = 0;
	short parsingSpeed = 0;
	long long time = timer_get_current_UTC();
	long long time2 = 0;
	XMLNode* node = nullptr;
	XMLNodeArgument* arg = nullptr;
	item_template* out = nullptr;
	for (size_t j = 0; j < mainnode->childNodes.size() - 1; j++)
	{

		if ((time2 = timer_get_current_UTC()) - time > 1)
		{
			time = time2;
			parsingSpeed = parsedPerSecond;
			parsedPerSecond = 0;
		}
		std::cout << "::Parsed <<[" << j << "] items. Parsing speed ~[" << parsingSpeed / 2 << "] Items per second." << "\r" << std::flush;

		out = new item_template;
		memset(out, 0, sizeof item_template);
		node = mainnode->childNodes[j];
		for (size_t i = 0; i < node->_arguments.size(); i++)
		{
			if ((arg = node->ConsumeArgument("id")))
			{
				out->id = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("name")))
			{
				out->name == arg->argumentValue.c_str();
			}
			else if ((arg = node->ConsumeArgument("coolTime")))
			{
				out->coolTime = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("category")))
			{
				out->category = itemCategoryDictionary[arg->argumentValue];
			}
			else if ((arg = node->ConsumeArgument("level")))
			{
				out->itemLevel = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("rank")))
			{
				out->rank = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("maxStack")))
			{
				out->maxStack = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("rareGrade")))
			{
				out->rareGrade = (e_item_rare_grade)atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("requiredEquipmentType")))
			{
				out->requiredEquipmentType = itemTypeDictionary[arg->argumentValue];
			}
			else if ((arg = node->ConsumeArgument("combatItemType")))
			{
				out->type = itemTypeDictionary[arg->argumentValue];
			}
			else if ((arg = node->ConsumeArgument("requiredLevel")))
			{
				out->requiredLevel = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("artisanable")))
			{
				out->artisanable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("dismantlable")))
			{
				out->dismantlable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("extractLook")))
			{
				//todo
			}
			else if ((arg = node->ConsumeArgument("guildWarehouseStorable")))
			{
				out->guildBankStorable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("masterpieceRate")))
			{
				out->masterpieceRate = (float)atof((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("obtainable")))
			{
				out->obtainable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("sellPrice")))
			{
				out->sellPrice = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("slotLimit")))
			{
				out->slotLimit = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("sortingNumber")))
			{
				out->sortingNumber = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("storeSellable")))
			{
				out->sellable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("tradable")))
			{
				out->tradable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("unidentifiedItemGrade")))
			{
				out->itemGrade = (e_item_grade)atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = node->ConsumeArgument("useOnlyTerritory")))
			{
				out->useOnlyTerritory = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("warehouseStorable")))
			{
				out->warehouseStoreable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("changeColorEnable")))
			{
				out->changeColorEnable = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("enchantEnable")))
			{
				out->enchantEnable = arg->argumentValue == "True" ? 1 : 0;
			}
			else if ((arg = node->ConsumeArgument("requiredRace")))
			{
				std::string chuck = "";
				for (size_t i = 0; i < arg->argumentValue.size(); i++)
				{
					if (arg->argumentValue[i] == ';')
					{
						out->requiredRace.push_back(playerRaceDictionary[chuck]);
						chuck.clear();
						continue;
					}
					chuck += arg->argumentValue[i];
				}
				if (chuck.size() > 0)
					out->requiredRace.push_back(playerRaceDictionary[chuck]);

				out->useOnlyByRace = 1;
			}
			else if ((arg = node->ConsumeArgument("boundType")))
			{
				out->bountType = itemBoundTypeDictionary[(const char*)arg->argumentValue.c_str()];
			}
			else if ((arg = node->ConsumeArgument("requiredClass")))
			{
				std::string chuck = "";
				for (size_t i = 0; i < arg->argumentValue.size(); i++)
				{
					if (arg->argumentValue[i] == ';')
					{
						out->requiredClasses.push_back(playerClassDictionary[chuck]);
						chuck.clear();
						continue;
					}
					chuck += arg->argumentValue[i];
				}
				if (chuck.size() > 0)
					out->requiredClasses.push_back(playerClassDictionary[chuck]);

				out->useOnlyByClass = 1;
			}
			else if ((arg = node->ConsumeArgument("linkMasterpiecePassivityCategoryId")))
			{
				out->masterpiecePassivityCategory = (passivity_category*)data_service::data_resolve_passivity_category(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("linkPassivityCategoryId")))
			{
				out->passivityCategory = (passivity_category*)data_service::data_resolve_passivity_category(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("linkMasterpieceEnchantId")))
			{
				out->masterpieceEnchant = (enchant_set *)data_service::data_resolve_enchant_set(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("linkPassivityId")))
			{
				std::string chuck = "";
				for (size_t i = 0; i < arg->argumentValue.size(); i++)
				{
					if (arg->argumentValue[i] == ';')
					{
						const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
						if (toInsert)
							out->passivities.push_back(toInsert);

						chuck.clear();
						continue;
					}
					chuck += arg->argumentValue[i];
				}
				if (chuck.size() > 0)
				{
					const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (toInsert)
						out->passivities.push_back(toInsert);

					chuck.clear();
				}
			}
			else if ((arg = node->ConsumeArgument("linkMasterpiecePassivityId")))
			{
				std::string chuck = "";
				for (size_t i = 0; i < arg->argumentValue.size(); i++)
				{
					if (arg->argumentValue[i] == ';')
					{
						const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
						if (toInsert)
							out->masterpiecePassivities.push_back(toInsert);

						chuck.clear();
						continue;
					}
					chuck += arg->argumentValue[i];
				}
				if (chuck.size() > 0)
				{
					const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (toInsert)
						out->masterpiecePassivities.push_back(toInsert);

					chuck.clear();
				}

			}
			else if ((arg = node->ConsumeArgument("linkCrestId")))
			{
				out->linkCrestId = arg->argumentValue;
			}
			else if ((arg = node->ConsumeArgument("linkCustomizingId")))
			{
				out->customizingItem = (cusotmizing_item*)data_service::data_resolve_customizing_item(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("linkEquipmentId")))
			{
				out->equipmentData = (equipment_data *)data_service::data_resolve_equipment_data(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("linkLookInfoId")))
			{
				out->linkLookInfoId = arg->argumentValue;
			}
			else if ((arg = node->ConsumeArgument("linkPetAdultId")))
			{
				out->linkPetAdultId = arg->argumentValue;
			}
			else if ((arg = node->ConsumeArgument("linkPetOrbId")))
			{
				out->linkPetOrbId = arg->argumentValue;
			}
			else if ((arg = node->ConsumeArgument("linkSkillId")))
			{
				out->linkSkillId = arg->argumentValue;
			}
			else if ((arg = node->ConsumeArgument("linkEnchantId")))
			{
				out->enchant = (enchant_set *)data_service::data_resolve_enchant_set(atoi(arg->argumentValue.c_str()));
			}
			else if ((arg = node->ConsumeArgument("changeLook")))
			{
				out->changeLook = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}



		}
		out_items.push_back(out);
		parsedPerSecond++;
	}

}
void XMLDocumentParser::BuildSkill(XMLNode * node, std::vector< const skill_template  *> &out_skills  /*not finished*/)
{
	int parsedPerSecond = 0;
	short parsingSpeed = 0;
	uint64 time = timer_get_current_UTC();
	uint64 time2 = 0;
	skill_template * out = nullptr;
	XMLNodeArgument * arg = nullptr;
	for (size_t i = 0; i < node->childNodes.size(); i++)
	{
		if ((time2 = timer_get_current_UTC()) - time > 1)
		{
			time = time2;
			parsingSpeed = parsedPerSecond;
			parsedPerSecond = 0;
		}
		std::cout << "::Parsed <<[" << i << "] skills. Parsing speed ~[" << parsingSpeed / 2 << "] Skills per second." << "\r" << std::flush;

		out = new skill_template;
		memset(out, 0, sizeof skill_template);
		XMLNode* skillNode = node->childNodes[i];

		if ((arg = skillNode->ConsumeArgument("id")))
		{
			out->id = atoi((const char*)arg->argumentValue.c_str());
			out->baseId = data_service::data_get_skill_base_id(out->id);
		}
		if ((arg = skillNode->ConsumeArgument("name")))
		{
			out->name = arg->argumentValue;
		}
		if ((arg = skillNode->ConsumeArgument("type")))
		{
			out->type = skillTypeDictionary[arg->argumentValue];
		}
		if ((arg = skillNode->ConsumeArgument("category")))
		{
			out->category = arg->argumentValue;
		}
		if ((arg = skillNode->ConsumeArgument("templateId")))
		{
			out->templateId = atoi(arg->argumentValue.c_str());
		}
		if ((arg = skillNode->ConsumeArgument("nextSkill")))
		{
			out->nextSkill = atoi(arg->argumentValue.c_str());
		}
		if ((arg = skillNode->ConsumeArgument("timeRate")))
		{
			out->timeRate = (float)atof(arg->argumentValue.c_str());
		}
		if ((arg = skillNode->ConsumeArgument("attackRange")))
		{
			out->attackRange = (float)atof(arg->argumentValue.c_str());
		}
		if ((arg = skillNode->ConsumeArgument("attackRangeMax")))
		{
			out->attackRangeMax = (float)atof(arg->argumentValue.c_str());
		}
		if ((arg = skillNode->ConsumeArgument("autoUse")))
		{
			out->autoUse = arg->argumentValue == "Ture" ? 1 : 0;
		}
		if ((arg = skillNode->ConsumeArgument("changeDirToCenter")))
		{
			out->changeDirToChanter = arg->argumentValue == "Ture" ? 1 : 0;
		}
		if ((arg = skillNode->ConsumeArgument("needWeapon")))
		{
			out->needWeapon = arg->argumentValue == "Ture" ? 1 : 0;
		}
		if ((arg = skillNode->ConsumeArgument("pushtarget")))
		{
			out->pushTarget = skillPushTargetDictionary[arg->argumentValue];
		}
		if ((arg = skillNode->ConsumeArgument("totalAtk")))
		{
			out->totalAttack = (float)atof(arg->argumentValue.c_str());
		}


		XMLNode * subNode = nullptr;
		for (size_t j = 0; j < skillNode->childNodes.size(); j++)
		{
			if ((subNode = skillNode->childNodes[j]))
			{
				if (subNode->tagName == "Property")
				{

				}
				else if (subNode->tagName == "Bullet")
				{

				}
				else if (subNode->tagName == "Action")
				{
					SkillAction  newAction = SkillAction();

					for (size_t k = 0; k < subNode->childNodes.size(); k++)
					{
						if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "MoveInvincible")
						{
							if ((arg = subNode->childNodes[k]->ConsumeArgument("endTime")))
								newAction.moveInvincivleEndTime = atoi(arg->argumentValue.c_str());

							if ((arg = subNode->childNodes[k]->ConsumeArgument("startTime")))
								newAction.moveInvincibleStartTime = atoi(arg->argumentValue.c_str());

						}
						else if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "Pending")
						{
							if ((arg = subNode->childNodes[k]->ConsumeArgument("startTime")))
								newAction.pendingStartTime = atoi(arg->argumentValue.c_str());
						}
						else if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "StageList")
						{
							XMLNode* stageNode = nullptr;
							for (size_t l = 0; l < subNode->childNodes[k]->childNodes.size(); l++)
							{
								if (!subNode->childNodes[k]->childNodes[l])
									continue;
								stageNode = subNode->childNodes[k]->childNodes[l];
								ActionStage  newStage = ActionStage();

								if ((arg = subNode->childNodes[k]->ConsumeArgument("changeDirToTarget")))
									newStage.changeDirToTarget = arg->argumentValue == "True" ? true : false;

								if ((arg = subNode->childNodes[k]->ConsumeArgument("movable")))
									newStage.movable = arg->argumentValue == "True" ? true : false;

								if ((arg = subNode->childNodes[k]->ConsumeArgument("changeDirToTargetMax")))
									newStage.changeDirToTargetMax = atoi(arg->argumentValue.c_str());

								if ((arg = subNode->childNodes[k]->ConsumeArgument("scriptId")))
									newStage.sriptId = atoi(arg->argumentValue.c_str());

								for (size_t b = 0; b < stageNode->childNodes.size(); b++)
								{
									if (stageNode->childNodes[b] && stageNode->childNodes[b]->tagName == "Property")
									{
										//if ((arg = stageNode->childNodes[b]->ConsumeArgument("animControlType"))) //todo

										if ((arg = stageNode->childNodes[b]->ConsumeArgument("adjustAnimToTarget")))
											newStage.pAdjustAnimToTarget = arg->argumentValue == "True" ? true : false;

									}
									else if (stageNode->childNodes[b] && stageNode->childNodes[b]->tagName == "AnimSeq")
									{
										if ((arg = stageNode->childNodes[b]->ConsumeArgument("animSet")))
										{
											std::string animSetName = arg->argumentValue;

											AnimSeq animSq = AnimSeq();
											if ((arg = stageNode->childNodes[b]->ConsumeArgument("duration")))
												animSq.duration = atoi(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("animRate")))
												animSq.animRate = (float)atof(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("blendInTime")))
												animSq.blendInTime = atoi(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("animMotionId")))
												animSq.animMotionId = atoi(arg->argumentValue.c_str());

											//if ((arg = stageNode->childNodes[b]->ConsumeArgument("bAnimLooping")))


											if ((arg = stageNode->childNodes[b]->ConsumeArgument("loopingRate")))
												animSq.loopingRate = (float)atof(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("rootMotionXYRate")))
												animSq.rootMotionXYRate = (float)atof(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("rootMotionZRate")))
												animSq.rootMotionZRate = (float)atof(arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("animName")))
												animSq.animation = data_service::data_resolve_animation(animSetName.c_str(), arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("movingAnimName")))
												animSq.movingAnimation = data_service::data_resolve_animation(animSetName.c_str(), arg->argumentValue.c_str());

											if ((arg = stageNode->childNodes[b]->ConsumeArgument("waitAnimName")))
												animSq.waitAnimation = data_service::data_resolve_animation(animSetName.c_str(), arg->argumentValue.c_str());

											if (animSq.animation && animSq.movingAnimation && animSq.waitAnimation)
												newStage.animationList.push_back(std::move(animSq));
										}
									}
								}

								newAction.stageList.push_back(std::move(newStage));
							}
						}
						//else if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "CameraType")
						//{
						//
						//}
						else if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "Cancel")
						{
							if ((arg = subNode->childNodes[k]->ConsumeArgument("frontCancelEndTime")))
								newAction.frontCancelEndTime = atoi(arg->argumentValue.c_str());

							if ((arg = subNode->childNodes[k]->ConsumeArgument("rearCancelStartTime")))
								newAction.rearCancelStartTime = atoi(arg->argumentValue.c_str());

							if ((arg = subNode->childNodes[k]->ConsumeArgument("moveCancelStartTime")))
								newAction.moveCancelStartTime = atoi(arg->argumentValue.c_str());
						}
					}

					out->actions.push_back(std::move(newAction));
				}
				else if (subNode->tagName == "TargetingList")
				{
					if ((arg = subNode->ConsumeArgument("adjustTargetingPos")))
					{
						out->adjustTargetingPos = adjustTargetingPosDictionary[arg->argumentValue];
					}

					for (size_t x = 0; x < subNode->childNodes.size(); x++)
					{
						XMLNode* targetingNode = nullptr;
						if ((targetingNode = subNode->childNodes[x]) && targetingNode->tagName == "Targeting")
						{
							Targeting  newTargeting = Targeting();

							if ((arg = targetingNode->ConsumeArgument("id")))
							{
								newTargeting.id = atoi(arg->argumentValue.c_str());
							}
							if ((arg = targetingNode->ConsumeArgument("type")))
							{
								newTargeting.type = targetingTypeDictionary[arg->argumentValue];
							}
							if ((arg = targetingNode->ConsumeArgument("time")))
							{
								newTargeting.time = atoi(arg->argumentValue.c_str());
							}
							if ((arg = targetingNode->ConsumeArgument("method")))
							{
								newTargeting.method = targetingMethodDictionary[arg->argumentValue];
							}
							if ((arg = targetingNode->ConsumeArgument("endTime")))
							{
								newTargeting.endTime = atoi(arg->argumentValue.c_str());
							}
							if ((arg = targetingNode->ConsumeArgument("startTime")))
							{
								newTargeting.startTime = atoi(arg->argumentValue.c_str());
							}
							if ((arg = targetingNode->ConsumeArgument("interval")))
							{
								newTargeting.interval = atoi(arg->argumentValue.c_str());
							}
							if ((arg = targetingNode->ConsumeArgument("until")))
							{
								newTargeting.until = atoi(arg->argumentValue.c_str());
							}


							for (size_t k = 0; k < targetingNode->childNodes.size(); k++)
							{
								if (targetingNode->childNodes[k] && targetingNode->childNodes[k]->tagName == "AreaList")
								{
									XMLNode *  areaNode = nullptr;
									for (size_t h = 0; h < targetingNode->childNodes[k]->childNodes.size(); h++)
									{
										bool validTarget = false;
										TargetingArea newTargetingArea = TargetingArea();
										memset(&newTargetingArea, 0, sizeof TargetingArea);

										if ((areaNode = targetingNode->childNodes[k]->childNodes[h]) && areaNode->tagName == "Area")
										{
											if ((arg = areaNode->ConsumeArgument("rotateAngle")))
											{
												newTargetingArea.rotateAngle = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("type")))
											{
												newTargetingArea.type = targetingAreaTypeDictionary[arg->argumentValue];
											}
											if ((arg = areaNode->ConsumeArgument("maxHeight")))
											{
												newTargetingArea.maxHeight = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("maxCount")))
											{
												newTargetingArea.maxCount = atoi(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("maxRadius")))
											{
												newTargetingArea.maxRadius = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("offsetDistance")))
											{
												newTargetingArea.offsetDistance = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("pierceDepth")))
											{
												newTargetingArea.pierceDepth = atoi(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("minHeight")))
											{
												newTargetingArea.minHeight = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("rangeAngle")))
											{
												newTargetingArea.rangeAngle = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("crosshairRadius")))
											{
												newTargetingArea.crosshairRadius = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("minRadius")))
											{
												newTargetingArea.minRadius = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("offsetAngle")))
											{
												newTargetingArea.offsetAngle = (float)atof(arg->argumentValue.c_str());
											}
											if ((arg = areaNode->ConsumeArgument("crosshairRadius2")))
											{
												newTargetingArea.crosshairRadius2 = (float)atof(arg->argumentValue.c_str());
											}
											validTarget = true;
										}
										else if ((areaNode = targetingNode->childNodes[k]->childNodes[h]) && areaNode->tagName == "Effect")
										{
											newTargetingArea.effect = AreaEffect();

											if ((arg = areaNode->ConsumeArgument("atk")))
												newTargetingArea.effect.attackMultiplier = (float)atof(arg->argumentValue.c_str());

											for (size_t f = 0; f < areaNode->childNodes.size(); f++)
											{
												if (areaNode->childNodes[f]->tagName == "HpDiff")
												{
													if ((arg = areaNode->childNodes[f]->ConsumeArgument("value")))
														newTargetingArea.effect.hpDiff = atoi(arg->argumentValue.c_str());
												}
												else if (areaNode->childNodes[f]->tagName == "MpDiff")
												{
													if ((arg = areaNode->childNodes[f]->ConsumeArgument("value")))
														newTargetingArea.effect.mpDiff = atoi(arg->argumentValue.c_str());
												}
												else if (areaNode->childNodes[f]->tagName == "AbnormalityOnCommon")
												{
													std::string chuck = "";
													if ((arg = areaNode->childNodes[f]->ConsumeArgument("id")))
													{
														for (size_t v = 0; v < arg->argumentValue.size(); v++)
														{
															if (arg->argumentValue[v] == ';')
															{
																const abnormality_template  * ab =data_service::data_resolve_abnormality(atoi(chuck.c_str()));
																if (ab)
																	newTargetingArea.effect.abnormalitiesOnCommon.push_back(ab);
																chuck.clear();
																continue;
															}
															else if (chuck.size() == arg->argumentValue.size() - 1)
															{
																const abnormality_template  * ab = data_service::data_resolve_abnormality(atoi(chuck.c_str()));
																if (ab)
																	newTargetingArea.effect.abnormalitiesOnCommon.push_back(ab);
															}
															chuck += arg->argumentValue[v];
														}

													}
												}
												else if (areaNode->childNodes[f]->tagName == "AbnormalityOnPvp")
												{
													std::string chuck = "";
													if ((arg = areaNode->childNodes[f]->ConsumeArgument("id")))
													{
														for (size_t v = 0; v < arg->argumentValue.size(); v++)
														{
															if (arg->argumentValue[v] == ';')
															{
																const abnormality_template  * ab = data_service::data_resolve_abnormality(atoi(chuck.c_str()));
																if (ab)
																	newTargetingArea.effect.abnormalitiesOnPvp.push_back(ab);
																chuck.clear();
																continue;
															}
															else if (chuck.size() == arg->argumentValue.size() - 1)
															{
																const abnormality_template  * ab = data_service::data_resolve_abnormality(atoi(chuck.c_str()));
																if (ab)
																	newTargetingArea.effect.abnormalitiesOnPvp.push_back(ab);
															}
															chuck += arg->argumentValue[v];
														}

													}
												}

											}
										}
										else if ((areaNode = targetingNode->childNodes[k]->childNodes[h]) && areaNode->tagName == "Reaction")
										{
											if ((arg = areaNode->ConsumeArgument("basicRate")))
												newTargetingArea.rectionBasicRate = (float)atof(arg->argumentValue.c_str());

											if ((arg = areaNode->ConsumeArgument("miniRate")))

												newTargetingArea.rectionMiniRate = (float)atof(arg->argumentValue.c_str());

										}
										else if ((areaNode = targetingNode->childNodes[k]->childNodes[h]) && areaNode->tagName == "HitEffect")
										{
											//need?
										}

										if (validTarget)
											newTargeting.areaList.push_back(std::move(newTargetingArea));
									}

								}
								else if (targetingNode->childNodes[k] && targetingNode->childNodes[k]->tagName == "Cost")
								{
									if ((arg = targetingNode->childNodes[1]->ConsumeArgument("hp")))
										newTargeting.cost.HP = atoi(arg->argumentValue.c_str());

									if ((arg = targetingNode->childNodes[1]->ConsumeArgument("mp")))
										newTargeting.cost.MP = atoi(arg->argumentValue.c_str());
								}
							}

							out->targetingList.push_back(std::move(newTargeting));
						}
					}
				}
				else if (subNode->tagName == "Defence")
				{

				}
				else if (subNode->tagName == "Drain")
				{

				}
				else if (subNode->tagName == "Precondition")
				{
					for (size_t x = 0; x < subNode->_arguments.size(); x++)
					{
						if ((arg = subNode->ConsumeArgument("coolTime")))
						{
							out->precondition.coolTime = atoi(arg->argumentValue.c_str());
						}
						else if ((arg = subNode->ConsumeArgument("modeChangeMethod")))
						{
							out->precondition.modeChangeMethod = atoi(arg->argumentValue.c_str());
						}
						else if ((arg = subNode->ConsumeArgument("modeNo")))
						{
							out->precondition.modeNo = atoi(arg->argumentValue.c_str());
						}
					}
					for (size_t x = 0; x < subNode->childNodes.size(); x++)
					{
						if (subNode->childNodes[x]->tagName == "Cost")
						{
							if ((arg = subNode->childNodes[x]->ConsumeArgument("hp")))
							{
								out->precondition.cost.HP = atoi(arg->argumentValue.c_str());
							}
							if ((arg = subNode->childNodes[x]->ConsumeArgument("mp")))
							{
								out->precondition.cost.MP = atoi(arg->argumentValue.c_str());
							}
						}
						else if (subNode->childNodes[x]->tagName == "ExclusiveAbnormality")
						{
							//todo check for args..
						}
					}
				}
				else if (subNode->tagName == "Projectile")
				{

				}

			}
		}

		out_skills.push_back(out);
		parsedPerSecond++;
	}

}
void XMLDocumentParser::BuildAbnormality(XMLNode * mainNode, std::vector< const abnormality_template  *>& out_abnormalities)
{
	int parsedPerSecond = 0;
	short parsingSpeed = 0;
	uint64 time = timer_get_current_UTC();
	uint64 time2 = 0;
	abnormality_template * out_ab = nullptr;
	XMLNodeArgument* arg = nullptr;
	XMLNode* abNode = nullptr;
	for (size_t j = 0; j < mainNode->childNodes.size(); j++)
	{
		if ((time2 = timer_get_current_UTC()) - time > 1)
		{
			time = time2;
			parsingSpeed = parsedPerSecond;
			parsedPerSecond = 0;
		}
		std::cout << "::Parsed <<[" << j << "] abnormalities. Parsing speed ~[" << parsingSpeed / 2 << "] Abnormalities per second." << "\r" << std::flush;
		abNode = mainNode->childNodes[j];
		XMLNodeArgument* arg = nullptr;
		out_ab = new abnormality_template;
		memset(out_ab, 0, sizeof abnormality_template);

		for (size_t i = 0; i < abNode->_arguments.size(); i++)
		{
			if ((arg = abNode->ConsumeArgument("id")))
			{
				out_ab->id = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("bySkillCategory")))
			{
				out_ab->bySkillCategory = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("infinity")))
			{
				out_ab->infinity = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = abNode->ConsumeArgument("isBuff")))
			{
				out_ab->isBuff = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = abNode->ConsumeArgument("isHideOnRefresh")))
			{
				out_ab->isHideOnRefresh = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = abNode->ConsumeArgument("isShow")))
			{
				out_ab->isShow = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = abNode->ConsumeArgument("kind")))
			{
				out_ab->kind = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("level")))
			{
				out_ab->level = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("notCareDeath")))
			{
				out_ab->notCareDeth = arg->argumentValue == "True" ? true : false;
			}
			else if ((arg = abNode->ConsumeArgument("priority")))
			{
				out_ab->priority = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("property")))
			{
				out_ab->property = atoi(arg->argumentValue.c_str());
			}
			else if ((arg = abNode->ConsumeArgument("time")))
			{
				out_ab->time = atoi(arg->argumentValue.c_str());
			}

			for (size_t i = 0; i < abNode->childNodes.size(); i++)
			{
				XMLNode * childNode = abNode->ConsumeChildNode("AbnormalityEffect");
				if (childNode)
				{
					XMLNodeArgument* arg1 = nullptr;
					abnormality_effect  new_effect;
					memset(&new_effect, 0, sizeof(new_effect));
					for (size_t j = 0; j < childNode->_arguments.size(); j++)
					{
						if ((arg1 = childNode->ConsumeArgument("type")))
						{
							new_effect.type = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("tickInterval")))
						{
							new_effect.tickInterval = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("value")))
						{
							new_effect.value = (float)atof(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("tickInterval")))
						{
							new_effect.tickInterval = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("isEnemyCheck")))
						{
							new_effect.isEnemyCheck = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("method")))
						{
							new_effect.method = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("appearEffectId")))
						{
							new_effect.appearEffectId = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("attackEffectId")))
						{
							new_effect.attackEffectId = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("overlayEffectId")))
						{
							new_effect.overlayEffectId = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("damageEffectId")))
						{
							new_effect.damageEffectId = atoi(arg1->argumentValue.c_str());
						}
						else if ((arg1 = childNode->ConsumeArgument("dissappearEffectId")))
						{
							new_effect.dissappearEffectId = atoi(arg1->argumentValue.c_str());
						}


						//this it for now
					}

					out_ab->effects.push_back(std::move(new_effect));
				}
			}
		}

		out_abnormalities.push_back(out_ab);
		parsedPerSecond++;
	}


}
void XMLDocumentParser::BuildPassivity(XMLNode * mainNode, std::vector< const passivity_template  *>& out_passivities)
{
	int parsedPerSecond = 0;
	short parsingSpeed = 0;
	uint64 time = timer_get_current_UTC();
	uint64 time2 = 0;
	int seconds = 0;
	passivity_template* out_p = nullptr;
	XMLNodeArgument* arg = nullptr;
	XMLNode* pasNode = nullptr;
	for (size_t j = 0; j < mainNode->childNodes.size(); j++)
	{
		if ((time2 = timer_get_current_UTC()) - time > 1)
		{
			time = time2;
			parsingSpeed = parsedPerSecond;
			parsedPerSecond = 0;
			seconds += 2;
		}
		std::cout << "::Parsed <<[" << j << "] passivities. Parsing speed ~[" << parsingSpeed / 2 << "] Passivities per second." << "\r" << std::flush;
		pasNode = mainNode->childNodes[j];
		out_p = new passivity_template;
		memset(out_p, 0, sizeof(passivity_template));

		for (size_t i = 0; i < pasNode->_arguments.size(); i++)
		{
			if ((arg = pasNode->ConsumeArgument("id")))
			{
				out_p->id = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("prob")))
			{
				out_p->probability = (float)atof((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("type")))
			{
				out_p->type = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("value")))
			{
				out_p->value = (float)atof((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("judgmentOnce")))
			{
				out_p->judgmentOnce = ((const char*)arg->argumentValue.c_str()) == "True" ? 1 : 0;
			}
			else if ((arg = pasNode->ConsumeArgument("kind")))
			{
				out_p->kind = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("mobSize")))
			{
				out_p->mobSize = mobSizeDictionary[(const char*)arg->argumentValue.c_str()];
			}
			else if ((arg = pasNode->ConsumeArgument("method")))
			{
				out_p->method = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("tickInterval")))
			{
				out_p->tickInterval = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("condition")))
			{
				out_p->condition = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("abnormalityCategory")))
			{
				out_p->abnormalityCategory = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("abnormalityKind")))
			{
				out_p->abnormalityKind = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("conditionValue")))
			{
				out_p->conditionValue = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("conditionCategory")))
			{
				out_p->conditionCategory = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("prevPassivityId")))
			{
				out_p->prevPassivityId = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("passivityChangeId")))
			{
				out_p->passivityChangeId = atoi((const char*)arg->argumentValue.c_str());
			}
			else if ((arg = pasNode->ConsumeArgument("passivityChangeTime")))
			{
				out_p->passivityChangeTime = atoi((const char*)arg->argumentValue.c_str());
			}
		}
		out_passivities.push_back(out_p);
		parsedPerSecond++;
	}
}
void XMLDocumentParser::BuildEquipmentSetData(XMLNode * eqNode, std::vector<const equipment_set_data  *>& out_equipmentSets)
{
	XMLNodeArgument* arg = nullptr;
	XMLNode* subNode = nullptr;
	int equipmentSetId = 0;
	equipment_set_data * newData = new equipment_set_data;
	memset(newData, 0, sizeof equipment_set_data);

	if ((arg = eqNode->ConsumeArgument("id")))
	{
		equipmentSetId = atoi(arg->argumentValue.c_str());
	}

	for (size_t i = 0; i < eqNode->childNodes.size(); i++)
	{
		if ((subNode = eqNode->childNodes[i]) && subNode->tagName == "Effect")
		{
			equipment_set_effect newEff;
			memset(&newEff, 0, sizeof newEff);
			for (size_t l = 0; l < subNode->_arguments.size(); l++)
			{
				if ((arg = subNode->ConsumeArgument("countOfEquipment")))
				{
					newEff.countOfEquipment = atoi(arg->argumentValue.c_str());
				}
				else if ((arg = subNode->ConsumeArgument("passiveList")))
				{
					std::string chuck;
					for (size_t k = 0; k < arg->argumentValue.size(); k++)
					{
						if (arg->argumentValue[k] == ',')
						{
							const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
							if (toInsert)
								newEff.passivities.push_back(toInsert);
							continue;
						}
						else if (k == arg->argumentValue.size() - 1)
						{
							const passivity_template * toInsert = data_service::data_resolve_passivity(atoi(chuck.c_str()));
							if (toInsert)
								newEff.passivities.push_back(toInsert);
							chuck = "";
							break;
						}
						chuck += arg->argumentValue[k];
					}
				}
			}

			newData->effects.push_back(std::move(newEff));
		}
		else if ((subNode = eqNode->childNodes[i]) && subNode->tagName == "ItemList")
		{
			for (size_t k = 0; k < subNode->childNodes.size(); k++)
			{
				if (subNode->childNodes[k] && subNode->childNodes[k]->tagName == "Item")
				{
					equipment_set_item newItem;
					memset(&newItem, 0, sizeof equipment_set_item);
					newItem.ids = new std::vector<uint32>();

					for (size_t l = 0; l < subNode->childNodes[k]->_arguments.size(); l++)
					{
						if ((arg = subNode->childNodes[k]->ConsumeArgument("id")))
						{
							std::string chuck;
							for (size_t k = 0; k < arg->argumentValue.size(); k++)
							{
								if (arg->argumentValue[k] == ';')
								{
									newItem.ids->push_back(atoi(chuck.c_str()));
									continue;
								}
								else if (k == arg->argumentValue.size() - 1)
								{
									newItem.ids->push_back(atoi(chuck.c_str()));
									chuck = "";
									break;
								}
								chuck += arg->argumentValue[k];
							}
						}

						else if ((arg = subNode->childNodes[k]->ConsumeArgument("equipPos")))
						{
							newItem.slotId = atoi(arg->argumentValue.c_str());
						}

						else if ((arg = subNode->childNodes[k]->ConsumeArgument("itemDelegatorName")))
						{
							//need?
						}
					}

					newData->items.push_back(std::move(newItem));
				}
			}
		}
	}

	if (newData)
	{
		newData->id = equipmentSetId;
		out_equipmentSets.push_back(newData);
	}
}
void XMLDocumentParser::BuildCustomizingItem(XMLNode * cNode, std::vector<const cusotmizing_item *>& out_costomizations)
{
	XMLNodeArgument* arg = nullptr;
	int id = 0;
	cusotmizing_item *newC = new cusotmizing_item;
	memset(newC, 0, sizeof cusotmizing_item);
	std::string chuck = "";
	for (size_t i = 0; i < cNode->_arguments.size(); i++)
	{
		if ((arg = cNode->ConsumeArgument("id")))
		{
			id = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = cNode->ConsumeArgument("passivityLink")))
		{
			for (size_t v = 0; v < arg->argumentValue.size(); v++)
			{
				chuck += arg->argumentValue[v];
				if (arg->argumentValue[v] == ';')
				{
					const passivity_template  * ab = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (ab)
						newC->passivities.push_back(ab);

					chuck.clear();
					continue;
				}
				else if (chuck.size() == arg->argumentValue.size())
				{
					const passivity_template  * ab = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (ab)
						newC->passivities.push_back(ab);
					break;
				}

			}

		}
		else if ((arg = cNode->ConsumeArgument("destroyProbOnDead")))
		{
			newC->destroyProbOnDeath = (float)(float)atof(arg->argumentValue.c_str());
		}
		else if ((arg = cNode->ConsumeArgument("takeSlot")))
		{
			newC->takeSlot = atoi(arg->argumentValue.c_str());
		}
	}

	if (id > 0)
	{
		newC->id = id;
		out_costomizations.push_back(newC);
	}
	else
	{
		delete newC;
		newC = nullptr;
	}
}
void XMLDocumentParser::BuildPassivityCategory(XMLNode * cNode, std::vector< const passivity_category *>& out_categories)
{
	XMLNodeArgument* arg = nullptr;
	int id = 0;
	passivity_category* newC = new passivity_category;
	memset(newC, 0, sizeof passivity_category);
	std::string chuck = "";
	for (size_t i = 0; i < cNode->_arguments.size(); i++)
	{
		if ((arg = cNode->ConsumeArgument("id")))
		{
			id = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = cNode->ConsumeArgument("passivityLink")))
		{

			for (size_t v = 0; v < arg->argumentValue.size(); v++)
			{
				chuck += arg->argumentValue[v];
				if (arg->argumentValue[v] == ',')
				{
					const passivity_template  * ab = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (ab)
						newC->passivities.push_back(ab);

					chuck.clear();
					continue;
				}
				else if (v == arg->argumentValue.size() - 1)
				{
					const passivity_template  * ab = data_service::data_resolve_passivity(atoi(chuck.c_str()));
					if (ab)
						newC->passivities.push_back(ab);
					break;
				}

			}
		}
		else if ((arg = cNode->ConsumeArgument("unchangeable")))
		{
			newC->unchangeable = arg->argumentValue == "True" ? true : false;
		}

	}

	if (id > 0)
	{
		newC->id = id;
		out_categories.push_back(newC);
	}
	else
	{
		delete newC;
		newC = nullptr;
	}

}
void XMLDocumentParser::BuildEquipmentData(XMLNode * eNode, std::vector<const equipment_data *>& out_data)
{
	XMLNodeArgument* arg = nullptr;
	int id = 0;
	equipment_data * newD = new equipment_data;
	memset(newD, 0, sizeof equipment_data);
	std::string chuck = "";
	for (size_t i = 0; i < eNode->_arguments.size(); i++)
	{
		if ((arg = eNode->ConsumeArgument("equipmentId")))
		{
			id = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = eNode->ConsumeArgument("passivityLinkG")))
		{
			newD->passivityG =data_service::data_resolve_passivity(atoi(arg->argumentValue.c_str()));
		}
		else if ((arg = eNode->ConsumeArgument("balance")))
		{
			newD->balance = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = eNode->ConsumeArgument("impact")))
		{
			newD->impact = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = eNode->ConsumeArgument("def")))
		{
			newD->defense = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = eNode->ConsumeArgument("minAtk")))
		{
			newD->minAttack = atoi(arg->argumentValue.c_str());
		}
		else if ((arg = eNode->ConsumeArgument("maxAtk")))
		{
			newD->maxAttack = atoi(arg->argumentValue.c_str());
		}

	}

	if (id > 0)
	{
		newD->id = id;
		out_data.push_back(newD);
	}
	else
	{
		delete newD;
		newD = nullptr;
	}
}
void XMLDocumentParser::BuildEnchantData(XMLNode * eNode, std::vector< const enchant_set *>& out_data)
{
	XMLNodeArgument* arg = nullptr;

	for (size_t j = 0; j < eNode->childNodes.size(); j++)
	{
		int id = 0;
		enchant_set * newD = new enchant_set();
		if (eNode->childNodes[j] && eNode->childNodes[j]->tagName == "Enchant")
		{
			XMLNode* subNode = eNode->childNodes[j];

			if ((arg = subNode->ConsumeArgument("id")))
				id = atoi(arg->argumentValue.c_str());

			for (size_t i = 0; i < subNode->childNodes.size(); i++)
			{
				XMLNode * subNode2 = subNode->childNodes[i];
				if (subNode2)
				{
					if (subNode2->tagName == "Effect")
					{
						enchant_effect  ee ;
						memset(&ee, 0, sizeof enchant_effect);

						for (size_t k = 0; k < subNode2->_arguments.size(); k++)
						{
							if ((arg = subNode2->ConsumeArgument("step")))
							{
								ee.step = atoi(arg->argumentValue.c_str());
							}
							else if ((arg = subNode2->ConsumeArgument("passivityCategoryId")))
							{
								ee.passivitiesCategory = data_service::data_resolve_passivity_category(atoi(arg->argumentValue.c_str()));
							}
						}

						newD->effects.push_back(std::move(ee));
					}
					else if (subNode2->tagName == "BasicStat")
					{
						basic_stat_enchant bse;
						memset(&bse, 0, sizeof basic_stat_enchant);

						for (size_t k = 0; k < subNode2->_arguments.size(); k++)
						{
							if ((arg = subNode2->ConsumeArgument("kind")))
							{
								bse.kind = basicStatEnchatKindDictionary[arg->argumentValue];
							}
							else if ((arg = subNode2->ConsumeArgument("rate")))
							{
								bse.rate = (float)atof(arg->argumentValue.c_str());
							}
							else if ((arg = subNode2->ConsumeArgument("enchantStep")))
							{
								bse.enchantStep = atoi(arg->argumentValue.c_str());
							}
						}
						newD->stats.push_back(std::move(bse));
					}

				}

			}

			if (id > 0)
			{
				newD->id = id;
				out_data.push_back(newD);
			}
			else
			{
				delete newD;
				newD = nullptr;
			}
		}
	}



}
void XMLDocumentParser::BuildNpcTemplate(XMLNode * tNode, int huntingZoneId, std::vector<const npc_template *>& out_data)
{
	npc_template * outTemplate = new npc_template;
	memset(outTemplate, 0, sizeof npc_template);
	outTemplate->huntingZoneId = huntingZoneId;
	XMLNodeArgument * arg = nullptr;
	if ((arg = tNode->ConsumeArgument("id")))
	{
		outTemplate->id = atoi(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("scale")))
	{
		outTemplate->scale = (float)atof(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("size")))
	{
		outTemplate->size = mobSizeDictionary[arg->argumentValue];
	}
	if ((arg = tNode->ConsumeArgument("gender")))
	{
		outTemplate->gender = npcGenderDictionary[arg->argumentValue];
	}
	if ((arg = tNode->ConsumeArgument("race")))
	{
		outTemplate->race = npcRaceDictionary[arg->argumentValue];
	}
	if ((arg = tNode->ConsumeArgument("resourceSize")))
	{
		outTemplate->resourceSize = atoi(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("resourceType")))
	{
		outTemplate->resourceType = npcResourceTypeDictionary[arg->argumentValue];
	}
	if ((arg = tNode->ConsumeArgument("basicActionId")))
	{
		outTemplate->basicActionId = atoi(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("elite")))
	{
		outTemplate->elite = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("isFreeNamed")))
	{
		outTemplate->isFreeNamed = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("showAggroTarget")))
	{
		outTemplate->showAggroTarget = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("showShorttermTarget")))
	{
		outTemplate->showShorttermTarget = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("villager")))
	{
		outTemplate->villager = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("unionElite")))
	{
		outTemplate->unionElite = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("isObjectNpc")))
	{
		outTemplate->isObjectNpc = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("isSpirit")))
	{
		outTemplate->isSpirit = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("spawnScriptId")))
	{
		outTemplate->spawnScriptId = atoi(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("despawnScriptId")))
	{
		outTemplate->despawnScriptId = atoi(arg->argumentValue.c_str());
	}
	if ((arg = tNode->ConsumeArgument("invincible")))
	{
		outTemplate->invincible = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("pushedByNpc")))
	{
		outTemplate->pushedByNpc = arg->argumentValue == "True" ? 1 : 0;
	}
	if ((arg = tNode->ConsumeArgument("isIgnoreSelect")))
	{
		outTemplate->isIgnoreSelect = arg->argumentValue == "True" ? 1 : 0;
	}

	XMLNode* sub = nullptr;
	for (size_t i = 0; i < tNode->childNodes.size(); i++)
	{
		sub = tNode->childNodes[i];
		if (sub && sub->tagName == "AltAnimation")
		{
			for (size_t j = 0; j < sub->childNodes.size(); j++)
			{
				if (sub->childNodes[j]->tagName == "AltAnim")
				{
					npc_alt_animation altAnim;
					memset(&altAnim, 0, sizeof altAnim);

					if ((arg = sub->childNodes[j]->ConsumeArgument("type")))
					{
						altAnim.type = npcAltAnimTypeDictionary[arg->argumentValue];
					}
					if ((arg = sub->childNodes[j]->ConsumeArgument("altAnimName")))
					{
						altAnim.animName = arg->argumentValue;
					}

					outTemplate->altAnimations.push_back(std::move(altAnim));
				}
			}
		}
		else if (sub && sub->tagName == "Stat")
		{
			if ((arg = sub->ConsumeArgument("level")))
			{
				outTemplate->stats.level = atoi(arg->argumentValue.c_str());
			}

			if ((arg = sub->ConsumeArgument("maxHp")))
			{
				outTemplate->stats.maxHp = atoi(arg->argumentValue.c_str());
			}
		}
		else if (sub && sub->tagName == "NamePlate")
		{
			if ((arg = sub->ConsumeArgument("nameplateHeight")))
			{
				outTemplate->namePlate.namePlateHeight = atoi(arg->argumentValue.c_str());
			}

			if ((arg = sub->ConsumeArgument("deathNameplateHeight")))
			{
				outTemplate->namePlate.deathNamePlateHeight = atoi(arg->argumentValue.c_str());
			}
		}
	}
	out_data.push_back(outTemplate);
}
void XMLDocumentParser::BuildAnimationSets(XMLNode * aNode, std::vector<const animation_set *>& out_data)
{
	XMLNode*setNode = nullptr;
	XMLNodeArgument* arg = nullptr;

	for (size_t i = 0; i < aNode->childNodes.size(); i++)
	{
		if ((setNode = aNode->childNodes[i]))
		{
			animation_set* newSet = new animation_set;
			memset(newSet, 0, sizeof animation_set);
			if ((arg = setNode->ConsumeArgument("name")))
				newSet->name = arg->argumentValue;
			else
			{
				delete newSet;
				newSet = nullptr;
				continue;
			}

			for (size_t j = 0; j < setNode->childNodes.size(); j++)
			{
				if (setNode->childNodes[j])
				{
					Animation newAnim;
					memset(&newAnim, 0, sizeof Animation);

					if ((arg = setNode->childNodes[j]->ConsumeArgument("name")))
					{
						newAnim.name = arg->argumentValue;

						if ((arg = setNode->childNodes[j]->ConsumeArgument("animDuration")))
							newAnim.animDuration = atoi(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance1")))
							newAnim.modeDistance[0] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance2")))
							newAnim.modeDistance[1] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance3")))
							newAnim.modeDistance[2] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance4")))
							newAnim.modeDistance[3] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance5")))
							newAnim.modeDistance[4] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance6")))
							newAnim.modeDistance[5] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("moveDistance7")))
							newAnim.modeDistance[6] = (float)atof(arg->argumentValue.c_str());

						if ((arg = setNode->childNodes[j]->ConsumeArgument("rootMotion")))
							newAnim.rootMotion = arg->argumentValue == "True" ? true : false;

						if ((arg = setNode->childNodes[j]->ConsumeArgument("rootRotate")))
							newAnim.rootRotate = arg->argumentValue == "True" ? true : false;

						if ((arg = setNode->childNodes[j]->ConsumeArgument("rotateYaw")))
							newAnim.rotateYaw = atoi(arg->argumentValue.c_str());

						newSet->animations.push_back(std::move(newAnim));
					}
					
				}
			}

			out_data.push_back(newSet);
		}
	}
}

std::map<std::string, e_item_category>			XMLDocumentParser::itemCategoryDictionary;
std::map<std::string, e_item_type>				XMLDocumentParser::itemTypeDictionary;
std::map<std::string, e_item_bound_type>		XMLDocumentParser::itemBoundTypeDictionary;
std::map<std::string, e_equipment_part>			XMLDocumentParser::equipmentPartDictionary;
std::map<std::string, e_player_class>			XMLDocumentParser::playerClassDictionary;
std::map<std::string, e_player_race>			XMLDocumentParser::playerRaceDictionary;

std::map<std::string, e_skill_type>				XMLDocumentParser::skillTypeDictionary;
std::map<std::string, e_push_target>			XMLDocumentParser::skillPushTargetDictionary;
std::map<std::string, e_targeting_area_type>	XMLDocumentParser::targetingAreaTypeDictionary;
std::map<std::string, e_targeting_method>		XMLDocumentParser::targetingMethodDictionary;
std::map<std::string, e_targeting_type>			XMLDocumentParser::targetingTypeDictionary;
std::map<std::string, e_adjust_targeting_pos>	XMLDocumentParser::adjustTargetingPosDictionary;

std::map<std::string, e_basic_stat_enchant_kind>XMLDocumentParser::basicStatEnchatKindDictionary;

std::map<std::string, e_npc_race>				XMLDocumentParser::npcRaceDictionary;
std::map<std::string, e_npc_gender>				XMLDocumentParser::npcGenderDictionary;
std::map<std::string, e_npc_resource_type>		XMLDocumentParser::npcResourceTypeDictionary;
std::map<std::string, e_npc_alt_anim_type>		XMLDocumentParser::npcAltAnimTypeDictionary;

std::map<std::string, e_mob_size>				XMLDocumentParser::mobSizeDictionary;
std::map<int, std::string>						XMLDocumentParser::regionsNames;

void XMLDocumentParser::InitItemsDictionary()
{
	itemBoundTypeDictionary.insert(std::pair<std::string, e_item_bound_type>("None", BOUND_NONE));
	itemBoundTypeDictionary.insert(std::pair<std::string, e_item_bound_type>("Equip", BOUND_EQUIP));
	itemBoundTypeDictionary.insert(std::pair<std::string, e_item_bound_type>("Pickup", BOUND_PICK_UP));
	itemBoundTypeDictionary.insert(std::pair<std::string, e_item_bound_type>("Loot", BOUND_LOOT));


	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("DISPOSAL", e_item_type::DISPOSAL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CUSTOM", e_item_type::CUSTOM));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("NO_COMBAT", e_item_type::NO_COMBAT));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHANGE_NAME", e_item_type::CHANGE_NAME));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_ACCESSORY", e_item_type::EQUIP_ACCESSORY));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("SKILLBOOK", e_item_type::SKILLBOOK));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("DUNGEON_COIN", e_item_type::DUNGEON_COIN));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CURSED_DREAM_PIECE", e_item_type::CURSED_DREAM_PIECE));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("BLESSED_DREAM_PIECE", e_item_type::BLESSED_DREAM_PIECE));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("UNIDENTIFY_SCROLL", e_item_type::UNIDENTIFY_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CURSE_REMOVE_SCROLL", e_item_type::CURSE_REMOVE_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("IDENTIFY_SCROLL", e_item_type::IDENTIFY_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHANGE_COLOR_ITEM", e_item_type::CHANGE_COLOR_ITEM));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("MIX_DISPOSAL", e_item_type::MIX_DISPOSAL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHANGE_RACE", e_item_type::CHANGE_RACE));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHANGE_GENDER", e_item_type::CHANGE_GENDER));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHANGE_LOOKS", e_item_type::CHANGE_LOOKS));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_WEAPON", e_item_type::EQUIP_WEAPON));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("START_GUILD_WAR", e_item_type::START_GUILD_WAR));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("GIVE_UP_GUILD_WAR", e_item_type::GIVE_UP_GUILD_WAR));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CHRONO_SCROLL", e_item_type::CHRONO_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("DUNGEON_RESET_SCROLL", e_item_type::DUNGEON_RESET_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("RESET_SCROLL", e_item_type::RESET_SCROLL));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("IMMEDIATE", e_item_type::IMMEDIATE));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_ARMOR_BODY", e_item_type::EQUIP_ARMOR_BODY));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_ARMOR_ARM", e_item_type::EQUIP_ARMOR_ARM));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_ARMOR_LEG", e_item_type::EQUIP_ARMOR_LEG));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("DOCUMENT", e_item_type::DOCUMENT));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CREST_RESET", e_item_type::CREST_RESET));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CREST_POINT", e_item_type::CREST_POINT));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("CREST", e_item_type::CREST));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("RECIPE", e_item_type::RECIPE));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_STYLE_ACCESSORY", e_item_type::EQUIP_STYLE_ACCESSORY));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_STYLE_WEAPON", e_item_type::EQUIP_STYLE_WEAPON));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_STYLE_BACK", e_item_type::EQUIP_STYLE_BACK));
	itemTypeDictionary.insert(std::pair<std::string, e_item_type>("EQUIP_STYLE_BODY", e_item_type::EQUIP_STYLE_BODY));
	//todo add ItemType to dictionary

	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("comabt", combat));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("magical", magical));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("custormaize", custormaize));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("pkitem", pkitem));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("medal", medal));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("service", service));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("earring", earring));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("necklace", necklace));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("ring", ring));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("skillbook", skillbook));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("token", token));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("order", order));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("etc", etc));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("petAdult", petAdult));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("petOrb", petOrb));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("dyeItem", dyeItem));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("dyeRaw", dyeRaw));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("dyeRecipe", dyeRecipe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("dual", dual));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("lance", lance));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("twohand", twohand));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("axe", axe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("circle", circle));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("bow", bow));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("staff", staff));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("rod", rod));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("gauntlet", gauntlet));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("shuriken", shuriken));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("blaster", blaster));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("accessoryHair", accessoryHair));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("weaponMaterial", weaponMaterial));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("armorMaterial", armorMaterial));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("keyItem", keyItem));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("generalMaterial", generalMaterial));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("fiber", fiber));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("metal", metal));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("alchemy", alchemy));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("leather", leather));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("alchemyMaterial", alchemyMaterial));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("fireworkMaterial", fireworkMaterial));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("weaponComponent", weaponComponent));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("armorComponent", armorComponent));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("bodyMail", bodyMail));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("handMail", handMail));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("feetMail", feetMail));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("bodyLeather", bodyLeather));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("handLeather", handLeather));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("feetLeather", feetLeather));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("bodyRobe", bodyRobe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("handRobe", handRobe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("feetRobe", feetRobe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("recipe", recipe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("quest", quest));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("document", document));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("crestPoint", crestPoint));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("crest", crest));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("charm", charm));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("extractRecipe", extractRecipe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("accessoryFace", accessoryFace));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_face", style_face));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_hair", style_hair));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_dual", style_dual));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_lance", style_lance));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_twohand", style_twohand));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_axe", style_axe));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_circle", style_circle));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_bow", style_bow));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_staff", style_staff));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("style_rod", style_rod));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("customize_wepon", customize_weapon));
	itemCategoryDictionary.insert(std::pair<std::string, e_item_category>("customize_armor", customize_armor));


	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("BELT", E_BELT));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("FINGER", E_FINGER));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("FEET", E_FEET));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("HAND", E_HAND));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("BODY", E_BODY));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("UNDERWEAR", E_UNDERWEAR));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("EAR", E_EAR));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("BROOCH", E_BROOCH));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("NECK", E_NECK));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("ACCESSORYFACE", E_ACCESSORYFACE));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("ACCESSORYHAIR", E_ACCESSORYHAIR));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("STYLE_FACE", E_STYLE_FACE));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("STYLE_BODY", E_STYLE_BODY));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("STYLE_WEAPON", E_STYLE_WEAPON));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("STYLE_BACK", E_STYLE_BACK));
	equipmentPartDictionary.insert(std::pair<std::string, e_equipment_part>("STYLE_HAIR", E_STYLE_HAIR));

	playerClassDictionary.insert(std::pair<std::string, e_player_class>("WARRIOR", WARRIOR));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("LANCER", LANCER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("SLAYER", SLAYER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("BERSERKER", BERSERKER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("SORCERER", SORCERER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("ARCHER", ARCHER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("PRIEST", PRIEST));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("MYSTIC", MYSTIC));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("REAPER", REAPER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("ENGINEER", ENGINEER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("FIGHTER", FIGHTER));
	playerClassDictionary.insert(std::pair<std::string, e_player_class>("ASSASSIN", ASSASSIN));

	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("HUMAN", HUMAN));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("HIGHELF", HIGHELF));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("AMAN", AMAN));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("CASTANIC", CASTANIC));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("ELIN", ELIN));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("POPORI", POPORI));
	playerRaceDictionary.insert(std::pair<std::string, e_player_race>("BARAKA", BARAKA));
}
void XMLDocumentParser::InitPassivityDictionary()
{
	mobSizeDictionary.insert(std::pair<std::string, e_mob_size>("Small", Small));
	mobSizeDictionary.insert(std::pair<std::string, e_mob_size>("Medium", Medium));
	mobSizeDictionary.insert(std::pair<std::string, e_mob_size>("Large", Large));
	mobSizeDictionary.insert(std::pair<std::string, e_mob_size>("All", All));
	mobSizeDictionary.insert(std::pair<std::string, e_mob_size>("Player", TPlayer));
	//all
}
void XMLDocumentParser::InitSkillsDictionary()
{
	skillPushTargetDictionary.insert(std::pair<std::string, e_push_target>("Pass", Pass));
	skillPushTargetDictionary.insert(std::pair<std::string, e_push_target>("Stop", Stop));
	skillPushTargetDictionary.insert(std::pair<std::string, e_push_target>("Push", Push));

	skillTypeDictionary.insert(std::pair<std::string, e_skill_type>("normal", normal));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("instance",instance));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("movingSkill",movingSkill));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("movingCharge",movingCharge));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("projectile",projectile));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("evade",evade));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("combo",combo));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("combo_instance",combo_instance));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("notimeline",notimeline));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("stand",stand));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("switch",switch_s));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("connect",connect_s));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("userslug",userslug));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("change",change));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("dash",dash));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("catchBack",catchBack));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("defence",defence_s));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("transform",transform));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("charm",charm_s));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("firewood",firewood));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("summon",summon));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("Gamble",Gamble));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("rebirth",rebirth));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("notimeline_revivenow",notimeline_revivenow));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("commander_scan",commander_scan));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("ShortTel",ShortTel));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("custom",custom));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("eventseed",eventseed));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("mount",mount));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("drain",drain));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("drain_back",drain_back));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("shortTel",shortTel));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("shootingmovingskill",shootingmovingskill));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("pressHit",pressHit));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("bulldozer",bulldozer));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("capture",capture));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("stone",stone));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("positionswap",positionswap));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("counter",counter));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("inversecapture",inversecapture));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("chaseEffect",chaseEffect));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("aiWork",aiWork));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("Projectile",Projectile));
	skillTypeDictionary.insert(std::pair<std::string,e_skill_type>("catchThrow",catchThrow));

	adjustTargetingPosDictionary.insert(std::pair < std::string, e_adjust_targeting_pos>("notUse", notUse));
}
void XMLDocumentParser::InitNpcTemplateDictionary()
{
	//todo
	//npcRaceDictionary
	//npcGenderDictionary
	//npcResourceTypeDictionary
	//npcAltAnimTypeDictionary
}


void XMLDocumentParser::ReleaseDictionaries()
{
	itemCategoryDictionary.clear();
	itemTypeDictionary.clear();
	itemBoundTypeDictionary.clear();
	equipmentPartDictionary.clear();
	playerClassDictionary.clear();
	playerRaceDictionary.clear();

	skillTypeDictionary.clear();
	skillPushTargetDictionary.clear();
	targetingAreaTypeDictionary.clear();
	targetingMethodDictionary.clear();
	targetingTypeDictionary.clear();
	adjustTargetingPosDictionary.clear();

	basicStatEnchatKindDictionary.clear();

	npcRaceDictionary.clear();
	npcGenderDictionary.clear();
	npcResourceTypeDictionary.clear();
	npcAltAnimTypeDictionary.clear();

	mobSizeDictionary.clear();
	regionsNames.clear();
}

//*******************************************************************************************************
XMLDocumentParser::XMLNode::XMLNode()
{
	parentNode = 0;
	consumed = 0;
}

XMLDocumentParser::XMLNode::~XMLNode()
{
	//clean arguments
	for (size_t i = 0; i < _arguments.size(); i++)
	{
		if (_arguments[i])
		{
			delete _arguments[i];
			_arguments[i] = 0;
		}
	}
	_arguments.clear();

	//clean childNodes
	for (size_t i = 0; i < childNodes.size(); i++)
	{
		if (childNodes[i])
		{
			delete childNodes[i];
			childNodes[i] = 0;
		}
	}
	childNodes.clear();

	parentNode = 0;
}

XMLDocumentParser::XMLNodeArgument * XMLDocumentParser::XMLNode::ConsumeArgument(STRING argumentName)
{
	XMLNodeArgument * out = nullptr;

	for (size_t i = 0; i < _arguments.size(); i++)
	{
		if (_arguments[i] && _arguments[i]->argumentName == argumentName && (_arguments[i]->consumed == 0x00))
		{
			_arguments[i]->consumed = 0x01;
			out = _arguments[i];
			break;
		}
	}

	return out;
}

XMLDocumentParser::XMLNode * XMLDocumentParser::XMLNode::ConsumeChildNode(STRING tagName)
{
	XMLNode *  out = nullptr;

	for (size_t i = 0; i < childNodes.size(); i++)
	{
		if (childNodes[i] && childNodes[i]->tagName == tagName && consumed == 0)
		{
			childNodes[i]->consumed = 1;
			out = childNodes[i];
			break;
		}
	}

	return out;
}


XMLDocumentParser::XMLNodeArgument::XMLNodeArgument()
{
	argumentName = argumentValue = "";
	consumed = 0;
}

XMLDocumentParser::XMLNodeArgument::~XMLNodeArgument()
{
	argumentName.clear();
	argumentValue.clear();
}
