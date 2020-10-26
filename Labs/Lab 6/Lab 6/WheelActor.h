#ifndef __WHEEL_H__
#define __WHEEL_H__
#include "common.h"
#include "Actor.h"

class WheelActor : public Actor
{
public:
	WheelActor(bool front, bool left) : front(front), left(left) {
		set_corner();
		set_idx();

		wheelnut = new CSemaphore(format("wheelnut_%s", corner.c_str()), 0, 1);
		wheelnut_done = new CSemaphore(format("wheelnut_%s_done", corner.c_str()), 0, 1);
		wheelnut_ready = new CSemaphore(format("wheelnut_%s_rdy", corner.c_str()), 0, 1);

		wheelremoval = new CSemaphore(format("wheelremoval_%s", corner.c_str()), 0, 1);
		wheelremoval_done = new CSemaphore(format("wheelremoval_%s_done", corner.c_str()), 0, 1);
		wheelremoval_ready = new CSemaphore(format("wheelremoval_%s_rdy", corner.c_str()), 0, 1);
		wheelremoval_start = new CSemaphore(format("wheelremoval_%s_start", corner.c_str()), 0, 1);

		wheelrefitting = new CSemaphore(format("wheelrefitting_%s", corner.c_str()), 0, 1);
		wheelrefitting_done = new CSemaphore(format("wheelrefitting_%s_done", corner.c_str()), 0, 1);
		wheelrefitting_ready = new CSemaphore(format("wheelrefitting_%s_rdy", corner.c_str()), 0, 1);
		wheelrefitting_start = new CSemaphore(format("wheelrefitting_%s_start", corner.c_str()), 0, 1);
		if (front)
			wheel_service_start = &wheel_f_service_start;
		else
			wheel_service_start = &wheel_b_service_start;
	}
	~WheelActor() {
		delete wheelnut;
		delete wheelnut_done;
		delete wheelnut_ready;

		delete wheelremoval;
		delete wheelremoval_done;
		delete wheelremoval_ready;
		delete wheelremoval_start;

		delete wheelrefitting;
		delete wheelrefitting_done;
		delete wheelrefitting_ready;
		delete wheelrefitting_start;
	}
protected:
	string corner;
	bool front;
	bool left;
	uint8_t idx;

	CSemaphore *wheel_service_start;

	CSemaphore *wheelnut;
	CSemaphore *wheelnut_done;
	CSemaphore *wheelnut_ready;

	CSemaphore *wheelremoval;
	CSemaphore *wheelremoval_done;
	CSemaphore *wheelremoval_ready;
	CSemaphore *wheelremoval_start;

	CSemaphore *wheelrefitting;
	CSemaphore *wheelrefitting_done;
	CSemaphore *wheelrefitting_ready;
	CSemaphore *wheelrefitting_start;
private:
	void set_corner();
	void set_idx();
};
#endif
