#ifndef SKILLTEMPLATE_H
#define SKILLTEMPLATE_H

#include "typeDefs.h"
#include "SkillEnums.h"

#include <vector>
#include <string>

struct abnormality_template;

struct Cost
{
	int HP;
	int MP;
};

struct Precondition
{

	int coolTime;
	Cost cost;
	int exclusiveAbnormality;
	int modeChangeMethod;
	int modeNo;
};

struct Animation
{
	std::string name;
	int
		animDuration,
		moveDir,
		rotateYaw;

	float modeDistance[6];

	bool
		rootMotion,
		rootRotate;
};

struct AnimSeq
{
	const Animation
		*animation,
		*movingAnimation,
		*waitAnimation;

	int
		duration,
		blendInTime,
		animMotionId;

	float
		animRate,
		loopingRate,
		rootMotionXYRate,
		rootMotionZRate;

};

struct ActionStage
{
	std::vector<AnimSeq> animationList;

	bool
		movable,
		changeDirToTarget,
		pAdjustAnimToTarget;

	int sriptId,
		changeDirToTargetMax,
		pAnimControlType; //tocollect

};

struct SkillAction
{
	//float
	//	cameraDuration,
	//	cameraStartTime,
	//	cameraWight;
	//
	//int 
	//	cameraType,
	//	cameraStartStageId,
	int
		moveInvincibleStartTime,
		moveInvincivleEndTime,

		pendingStartTime,

		frontCancelEndTime,
		moveCancelStartTime,
		rearCancelStartTime;

	std::vector<ActionStage> stageList;

};

struct ChargeStage
{

	float costHPRate;
	float costMPRate;
	float duration;
	int shotSkillID;
};

struct CharginStageList
{
	std::vector<ChargeStage> chargeStageList;
	int costTotalHP;
	int costTotalMP;

	float intervalCostHPRate;
	float intervalCostMPRate;
	bool movable;
};

struct AttackTarget
{
	long long id;
	float x, y, z;
};

struct SkillArgs
{
	float x, y, z, x2, y2, z2;
	short heading;
	byte unk1, unk2, unk3, isInstance, isInstanceCombo;
	long long target;

	std::vector<AttackTarget> targets;
};

struct AreaEffect
{
	float
		attackMultiplier;

	int
		hpDiff,
		mpDiff;

	std::vector<const abnormality_template *> abnormalitiesOnPvp;
	std::vector<const abnormality_template *> abnormalitiesOnCommon;
};

struct TargetingArea
{
	int
		dropItem,
		pierceDepth,
		maxCount;

	float
		crosshairRadius,
		crosshairRadius2,
		maxHeight,
		maxRadius,
		minHeight,
		minRadius,
		offsetAngle,
		offsetDistance,
		rangeAngle,
		rectionBasicRate,
		rectionMiniRate,
		rotateAngle;


	e_targeting_area_type type;
	AreaEffect effect;
};

struct Targeting
{
	std::vector<TargetingArea> areaList;
	Cost cost;
	int endTime;
	int id;
	int interval;
	e_targeting_method method;
	e_targeting_type type;
	int startTime;
	int time;
	int until;
	//std::vector<ProjectileSkill> projectileSkillList;
};

struct ProjectileData
{
	int areaBoxSizeX;
	int areaBoxSizeY;
	int areaBoxSizeZ;
	int lifeTime;
	std::vector<Targeting> targetingList;
};

struct AreaCircle
{
	float radius, x, y, z;
};

struct AttackResult
{
	byte critical;

	unsigned int
		attackerPID,
		attackerSID,

		targetPID,
		targetSID;

	int
		hpDiff,
		angleDiff,
		damage;

	e_attack_type type;
	e_visual_effect visualEffect;


	unsigned int uid;
};

typedef struct animation_set
{
	std::string name;
	std::vector<Animation> animations;

}animation_set , *lpanimation_set;

typedef struct skill_template
{
	std::vector<Targeting> targetingList;

	int
		nextSkill,
		templateId,
		id,
		baseId,
		level,
		needSkill,
		parentId,
		totalStk,
		totalStkPvP;

	std::vector<SkillAction> actions;
	std::string
		category,
		name;

	float
		attackRangeMax,
		attackRange,
		totalAttack,
		timeRate,
		totalAtk;

	bool
		autoUse,
		needWeapon,
		changeDirToChanter,
		keepMovingCharge,
		keptMovingCharge,
		useSkillWhileReaction;


	e_skill_type					type;
	e_push_target					pushTarget;
	Animation						animation;
	CharginStageList				chargingStageList;
	Precondition					precondition;
	ProjectileData					projectileData;
	e_adjust_targeting_pos			adjustTargetingPos;


} skill_template, *lpskill_template;

#endif 
