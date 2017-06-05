#pragma once

#include <cstdint>
#include <unordered_map>

#include "list.hpp"

#define MAPLAYERS 3 // number of layers contained in a single map
#define OBSTACLELAYER 1 // obstacle layer in map

typedef struct map_t
{
	char name[32];   // name of the map
	char author[32]; // author of the map
	unsigned int width, height;  // size of the map
	int32_t* tiles;
	std::unordered_map<int32_t, node_t*> entities_map;
	list_t* entities;
} map_t;

int generateDungeon(char* levelset, uint32_t seed);
void assignActions(map_t* map);

extern map_t map;
