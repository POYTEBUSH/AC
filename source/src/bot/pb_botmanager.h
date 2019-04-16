#pragma once

#include "pb_marpo.h"
#include "entity.h"
#include "bot.h"

#include "pb_FuzzyModule.h"
#include "pb_FuzzyTerm.h"

#include "tools.h"

class pb_botmanager
{

public:
	
	static pb_botmanager* Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new pb_botmanager();
			mInstance->Init();
		}
		return mInstance;
	}

	void Init();
	void Update(vector<botent*> bots);

private:

	pb_botmanager() {};
	~pb_botmanager() {};

	pb_FuzzyModule mFuzzyModule;

	static pb_botmanager* mInstance;

};

