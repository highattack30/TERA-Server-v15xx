#ifndef DATASERVICE_H
#define DATASERVICE_H
#define _CRT_SECURE_NO_WARNINGS

#include "typeDefs.h"

#include <vector>
#include <map>

struct animation_set;
struct abnormality_template;
struct passivity_template;
struct area_template;
struct equipment_set_data;
struct cusotmizing_item;
struct passivity_category;
struct equipment_data;
struct enchant_set;
struct item_template;
struct npc_template;
struct animation_set;
struct skill_template;

struct Animation;

class data_service
{
public:
	static std::vector<const area_template *>			_areas;
	static std::vector<const passivity_template *>		_passivities;
	static std::vector<const abnormality_template *>	_abnormalities;
	static std::vector<const passivity_category *>		_passivity_categories;
	static std::vector<const npc_template * >			_npcs;
	static std::vector<const animation_set *>			_animations;
	static std::vector<const equipment_set_data *>		_equipment_sets;
	static std::vector<const equipment_data *>			_equipment_data;
	static std::vector<const enchant_set * >			_enchant_sets;
	static std::vector<const cusotmizing_item *>		_customizing_items;
	static std::vector<const item_template*>			_items;
	static std::vector<const animation_set*>			_animation_sets;


	static std::vector<const skill_template *>			_warrior;
	static std::vector<const skill_template *>			_archer;
	static std::vector<const skill_template *>			_berserker;
	static std::vector<const skill_template *>			_slayer;
	static std::vector<const skill_template *>			_sorcerer;
	static std::vector<const skill_template *>			_lancer;
	static std::vector<const skill_template *>			_priest;
	static std::vector<const skill_template *>			_mystic;
	static std::vector<const skill_template *>			_reaper;
	static std::vector<const skill_template *>			_gunner;
	static std::vector<const skill_template *>			_brawler;
	static std::vector<const skill_template *>			_ninja;

	static bool										data_init();
	static void										data_release();

	static passivity_template*						data_resolve_passivity(uint32 id);
	static passivity_category*						data_resolve_passivity_category(uint32 id);
	static item_template*							data_resolve_item(uint32);
	static abnormality_template *					data_resolve_abnormality(uint32 id);
	static skill_template *							data_resolve_skill(uint32 id);
	static enchant_set *							data_resolve_enchant_set(uint32 id);
	static equipment_data *							data_resolve_equipment_data(uint32 id);
	static cusotmizing_item *						data_resolve_customizing_item(uint32 id);

	static  Animation  *							data_resolve_animation(STRING animation_set_name, STRING animation_name);

	static uint32									data_get_skill_base_id(uint32 skill_id);

private:
	static bool data_load_abnormalities();
	static bool data_load_passivities();
	static bool data_load_skills();
	static bool data_load_items();
	static bool data_load_npcs();
	static bool data_load_equipment();

	static bool data_load_equipment_sets(STRING file);
	static bool data_load_customizing_items(STRING file);
	static bool data_load_equipment_data(STRING file);
	static bool data_load_enchant_data(STRING file);


	static void data_equipment_final_build();
};


#endif
