#include "cube.h"
#include "pb_target.h"


pb_target::pb_target()
{
	mTargetCompletionRange = 0.f;
	mTargetType = ETargetType::TARGET_TYPE_NONE;
	mTargetEntity = nullptr;
	mTargetBot = nullptr;
}

pb_target::~pb_target() {}

void pb_target::Set(const vec & position)
{
	mTargetVec = position;
	mTargetType = ETargetType::TARGET_TYPE_NONE;	
}

void pb_target::Set(const entity * entity)
{
	mTargetEntity = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = vec(entity->x, entity->y, entity->z);

	mTargetType = ETargetType::TARGET_TYPE_ENTITY;
}

void pb_target::Set(const playerent * entity)
{
	mTargetBot = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = entity->o;

	mTargetType = ETargetType::TARGET_TYPE_BOT;
}
