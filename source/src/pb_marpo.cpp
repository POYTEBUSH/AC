#include "cube.h"
#include "pb_marpo.h"

std::stack<pb_task*> pb_marpo::mLongTermTasks;
std::stack<pb_task*> pb_marpo::mReactiveTasks;
std::stack<pb_task*> pb_marpo::mImmediateTasks;

pb_task* pb_marpo::mMainTasks;