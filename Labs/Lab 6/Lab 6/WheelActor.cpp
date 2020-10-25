#include "WheelActor.h"

void WheelActor::set_corner()
{
	string f, l;
	if (front)
		f = "front";
	else
		f = "back";
	if (left)
		l = "left";
	else
		l = "right";
	corner = format("%s_%s", f.c_str(), l.c_str());
}

void WheelActor::set_idx()
{
	idx = 0;
	if (front)
		idx |= 1;
	if (left)
		idx |= 1 << 1;
}
