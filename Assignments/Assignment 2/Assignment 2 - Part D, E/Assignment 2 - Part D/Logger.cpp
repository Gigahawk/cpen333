#include "Logger.h"
LogMgr* LogMgr::inst = nullptr;

void show_log(bool enable)
{
	LogMgr* lm = LogMgr::get_instance();
	lm->enable = enable;
}
