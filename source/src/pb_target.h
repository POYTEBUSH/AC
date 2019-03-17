#pragma once
#include "geom.h"
#include "entity.h"

enum ETargetType 
{
	TARGET_TYPE_ENTITY,
	TARGET_TYPE_BOT,
	TARGET_TYPE_NONE
};

class pb_target
{
public:
	pb_target();
	~pb_target();

	void Set(const vec& position);
	void Set(const entity* entity);
	void Set(const playerent* entity);


private:

	//Position of target
	vec mTargetVec;

	//Target entity if targeting a collectable
	const entity* mTargetEntity;
	//Target entity when targeting a bot or player
	const playerent* mTargetBot;

	//Type of target
	ETargetType mTargetType;

	//Range within where the target is regarded as obtained
	float mTargetCompletionRange;
};

