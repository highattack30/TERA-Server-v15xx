#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define XML_DEBUG 1

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "typeDefs.h"
#include "stringUtils.h"
#include "PlayerEnums.h"
#include "creatureEnums.h"
#include "NpcEnum.h"
#include "equipmentEnums.h"
#include "ItemEnums.h"
#include "SkillEnums.h"

struct abnormality_template;
struct passivity_template;
struct area_template;
struct equipment_set_data;
struct cusotmizing_item;
struct passivity_category;
struct equipment_data;
struct enchant_set;
struct npc_template;
struct animation_set;
struct skill_template;
struct item_template;

class XMLDocumentParser
{
public:
	class XMLNodeArgument
	{
	public:
		XMLNodeArgument();
		~XMLNodeArgument();
		std::string argumentName;
		std::string argumentValue;
		byte	consumed;
	};
	class XMLNode
	{
	public:
		XMLNode();
		~XMLNode();
		XMLNodeArgument * ConsumeArgument(STRING argumentName);
		std::vector<XMLNodeArgument * > _arguments;
		XMLNode* ConsumeChildNode(STRING tagName);

		std::string tagName;
		XMLNode* parentNode;
		std::vector< XMLNode*> childNodes;
		byte consumed;
	};
public:
	static const bool ParseAreaXMLDocument(const char* fileName, std::vector< const area_template *>& out_areas);
	static const bool ParseItemsXMLDocument(const char* fileName, std::vector<const item_template *>& out_items);
	static const bool ParseSkillXMLDocument(const char* fileName, std::vector<const skill_template  *>& out_skills);
	static const bool ParseAbnormalityXMLDocument(const char* fileName, std::vector<const abnormality_template *>& out_abnormalities);
	static const bool ParsePassivityXMLDocument(const char* fileName, std::vector<const passivity_template *>& out_passivities);
	static const bool ParseEquipmentSetXMLDocument(const char * fileName, std::vector<const equipment_set_data *>& out_equipmentSets);
	static const bool ParseCustomizingItmesXMLDocument(const char * fileName, std::vector< const cusotmizing_item *>& out_costomizations);
	static const bool ParsePassivityCategoriesXMLDocument(const char * fileName, std::vector<const passivity_category *>& out_categories);
	static const bool ParseEquipmentDataXMLDocument(const char * fileName, std::vector< const equipment_data *>& out_data);
	static const bool ParseEnchantDataXMLDocument(const char * fileName, std::vector<const enchant_set *>& out_data);
	static const bool ParseRegionStrSheetXMLDocument(const char* fileName);
	static const bool ParseNpcTemplateXMLDocuemnt(const char* fileName, std::vector<const npc_template  *>&out_data);
	static const bool ParseAnimationDataXMLDocument(const char* fileName, std::vector<const animation_set *> & out_data);

	static void ReleaseDictionaries();
private:
	static const byte ParseXMLDocument(std::vector<XMLNode*>& out_nodes, const char* fileName);
	static int GetNode(char* buffer, int mainCursor, XMLNode* outNode);


	static void BuildRegionName(XMLNode* node);
	static void BuildAreas(XMLNode* mainNode, std::vector<const area_template *>& out_areas);
	static void BuildItems(XMLNode* node, std::vector<const item_template *> & out_items);
	static void BuildSkill(XMLNode* skillNode, std::vector< const skill_template  *> &out_skills);
	static void BuildAbnormality(XMLNode * abNode, std::vector< const abnormality_template  *>& out_abnormalities);
	static void BuildPassivity(XMLNode *pasNode, std::vector< const passivity_template  *>& out_passivities);
	static void BuildEquipmentSetData(XMLNode *eqNode, std::vector<const equipment_set_data  *>& out_equipmentSets);
	static void BuildCustomizingItem(XMLNode * cNode, std::vector<const cusotmizing_item *>& out_costomizations);
	static void BuildPassivityCategory(XMLNode * cNode, std::vector<const passivity_category *>& out_categories);
	static void BuildEquipmentData(XMLNode * eNode, std::vector<const equipment_data *>& out_data);
	static void BuildEnchantData(XMLNode * eNode, std::vector<const enchant_set *>& out_data);
	static void BuildNpcTemplate(XMLNode* tNode, int huntingZoneId, std::vector<const npc_template  *>&out_data);
	static void BuildAnimationSets(XMLNode * aNode, std::vector<const animation_set *> & out_data);

	static void InitItemsDictionary();
	static void InitPassivityDictionary();
	static void InitSkillsDictionary();
	static void InitNpcTemplateDictionary();

	static std::map<std::string, e_item_category>		itemCategoryDictionary;
	static std::map<std::string, e_item_type>			itemTypeDictionary;
	static std::map<std::string, e_item_bound_type>			itemBoundTypeDictionary;
	static std::map<std::string, e_equipment_part>		 equipmentPartDictionary;
	static std::map<std::string, e_player_class>		playerClassDictionary;
	static std::map<std::string, e_player_race>			playerRaceDictionary;

	static std::map<std::string, e_skill_type>					skillTypeDictionary;
	static std::map<std::string, e_push_target>						skillPushTargetDictionary;
	static std::map<std::string, e_targeting_area_type>			targetingAreaTypeDictionary;
	static std::map<std::string, e_targeting_method>				targetingMethodDictionary;
	static std::map<std::string, e_targeting_type>				targetingTypeDictionary;
	static std::map<std::string, e_adjust_targeting_pos> adjustTargetingPosDictionary;

	static std::map<std::string, e_basic_stat_enchant_kind> basicStatEnchatKindDictionary;

	static std::map<std::string, e_npc_race>			npcRaceDictionary;
	static std::map<std::string, e_npc_gender>				npcGenderDictionary;
	static std::map<std::string, e_npc_resource_type>		npcResourceTypeDictionary;
	static std::map<std::string, e_npc_alt_anim_type>			 npcAltAnimTypeDictionary;


	static std::map<std::string, e_mob_size> mobSizeDictionary;
	static std::map<int, std::string> regionsNames;
};

