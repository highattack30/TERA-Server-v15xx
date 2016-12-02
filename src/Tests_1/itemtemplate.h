#ifndef ITEMTEMPLATE_H
#define ITEMTEMPLATE_H

#include "typeDefs.h"
#include "ItemEnums.h"
#include "playerEnums.h"

#include <vector>

struct passivity_template;
struct passivity_category;
struct cusotmizing_item;
struct equipment_data;
struct equipment_set_data;
struct enchant_set;

typedef struct item_template
{
	std::vector<e_player_class> requiredClasses;
	std::vector<e_player_race> requiredRace;

	item_id id;
	uint32
		minAtk,
		maxAtk,
		impact,
		def,
		sortingNumber,
		itemLevel,
		requiredLevel,
		passivityLinkG,
		maxStack,
		rank,
		coolTimeGroup,
		slotLimit,
		buyPrice,
		sellPrice,
		countOfSlot,
		coolTime;

	e_equipment_part		part;
	e_item_type			combatItemType;
	e_item_category		category;
	e_item_type			type;
	e_item_type			requiredEquipmentType;
	e_item_grade			itemGrade;
	e_item_rare_grade		rareGrade;
	e_item_bound_type		bountType;

	float
		masterpieceRate;

	byte
		changeColorEnable,
		changeLook,
		tradable,
		warehouseStoreable,
		destroyable,
		dismantlable,
		sellable,
		obtainable,
		enchantEnable,
		artisanable,
		useOnlyByGender,
		useOnlyTerritory,
		useOnlyByRace,
		hasBonus,
		bankStorable,
		guildBankStorable,
		useOnlyByClass;

	std::string
		linkCrestId,
		linkLookInfoId,
		linkPetAdultId,
		linkPetOrbId,
		name,
		linkSkillId;


	passivity_category* masterpiecePassivityCategory;
	passivity_category* passivityCategory;

	cusotmizing_item* customizingItem;

	equipment_set_data * equipmentSetData;

	equipment_data  * equipmentData;

	// if enchatLevel <= 9 [_enchant] else if enchantLeve >=10 [_masterpieceEnchant]
	enchant_set * enchant;
	enchant_set * masterpieceEnchant;

	// if enchatLevel <= 9 [_passivities] else if enchantLeve >=10 [_masterpiecePassivities]
	std::vector<const passivity_template *> passivities; //satic passivities
	std::vector<const passivity_template *> masterpiecePassivities; //satic msterwork passivities

}item_template ,*  lpitem_template;
#endif
