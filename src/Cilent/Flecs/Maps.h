#pragma once

#include <flecs.h>

#define map_new(type, count) (ecs_map_new(type*, 0, count))
#define map_get(map, key, type) ((type*)(ecs_map_get(map, type*, Cilent_Hash_String(key))))
#define map_set(map, key, value) (ecs_map_set(map, Cilent_Hash_String(key), value))

uint64_t Cilent_Hash_String(const char* str);
