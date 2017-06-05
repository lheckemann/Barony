/*-------------------------------------------------------------------------------

	BARONY
	File: collision.hpp
	Desc: collision.cpp header file

	Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
	See LICENSE for details.

-------------------------------------------------------------------------------*/

#pragma once

#define IGNORE_ENTITIES 1

// function prototypes
float entityDist(Entity* my, Entity* your);
Entity* entityClicked();
bool entityInsideTile(Entity* entity, int x, int y, int z);
bool entityInsideEntity(Entity* entity1, Entity* entity2);
bool entityInsideSomething(Entity* entity);
int barony_clear(float tx, float ty, Entity* my);
float clipMove(float* x, float* y, float vx, float vy, Entity* my);
Entity* findEntityInLine(Entity* my, float x1, float y1, float angle, int entities, Entity* target);
float lineTrace(Entity* my, float x1, float y1, float angle, float range, int entities, bool ground);
float lineTraceTarget(Entity* my, float x1, float y1, float angle, float range, int entities, bool ground, Entity* target); //If the linetrace function encounters the linetrace entity, it returns even if it's invisible or passable.
int checkObstacle(long x, long y, Entity* my, Entity* target);
