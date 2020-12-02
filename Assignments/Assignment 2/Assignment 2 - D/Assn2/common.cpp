#include "common.h"

uint16_t id_from_str(string username)
{
    hash<string> hasher;
    return hasher(username);
}
