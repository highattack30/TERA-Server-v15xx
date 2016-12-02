#pragma once


enum e_push_target : unsigned char
{
	Pass = 0,
	Stop = 1,
	Push = 2
};

enum e_skill_type : short
{
	instance = 0,
	movingSkill = 1,
	normal = 2,
	movingCharge = 3,
	projectile = 4,
	evade = 5,
	combo_instance = 6,
	notimeline = 7,
	stand = 8,
	combo = 9,
	switch_s = 10,
	connect_s = 11,
	userslug = 12,
	change = 13,
	dash = 14,
	catchBack = 15,
	defence_s = 16,
	transform = 17,
	charm_s = 18,
	firewood = 19,
	summon = 20,
	Gamble = 21,
	rebirth = 22,
	notimeline_revivenow = 23,
	commander_scan = 24,
	ShortTel = 25,
	custom = 26,
	eventseed = 27,
	mount = 28,
	drain = 29,
	drain_back = 30,
	shortTel = 31,
	shootingmovingskill = 32,
	pressHit = 33,
	bulldozer = 34,
	capture = 35,
	stone = 36,
	positionswap = 37,
	counter = 38,
	inversecapture = 39,
	chaseEffect = 40,
	aiWork = 41,
	Projectile = 42,
	catchThrow = 43,
};

enum e_targeting_area_type : int
{
	EnemyOrPvP = 0,
	Me = 1,
	Enemy = 2,
	PvP = 3,
	AAll = 4,
	AllIncludeVillager = 5,
	AllyOnDeath = 6,
	AllyVillager = 7,
	Ally = 8,
	Party = 9,
	AllyExceptMe = 10,
	PartyExceptMe = 11,
	AllExceptMe = 12,
	No = 13
};

enum e_adjust_targeting_pos
{
	notUse,
};

enum e_targeting_type : short
{
	None
};

enum e_targeting_method :short
{
	NNormal = 0,
	LockOn = 1,
	UseCollected = 2,
	RealForward = 3,
	SpiritAttack = 4,
	SpiritFollow = 5
};

enum e_attack_type : unsigned char //todo :test
{
	DAMAGE =1,
	HEAL =2,
	MANA_REGEN = 3
};

enum e_visual_effect : unsigned int
{

};



