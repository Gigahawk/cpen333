#ifndef __ACTOR_H__
#define __ACTOR_H__
#include "rt.h"
#include "Semaphores.h"

// Avoid circular imports
class Driver;

class Actor : public ActiveClass, public Semaphores, public CDataPool
{
private:
	struct global_data_t {
		Driver* curr_driver;
	};

public:
	Actor() :
		CDataPool("race_glob_data", sizeof(struct global_data_t))
	{
		global_data = (struct global_data_t*)(LinkDataPool());
		curr_driver = &(global_data->curr_driver);
		state = "Idle";
	}
	string inline get_state() { return state; };
protected:
	struct global_data_t* global_data;
	Driver** curr_driver;
	string state;
};
#endif
