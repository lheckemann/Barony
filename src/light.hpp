#pragma once

typedef struct light_t
{
	int32_t x, y;
	int32_t radius;
	int32_t intensity;
	int32_t* tiles;

	// a pointer to the light's location in a list
	node_t* node;
} light_t;

light_t* lightSphereShadow(int32_t x, int32_t y, int32_t radius, int32_t intensity);
light_t* lightSphere(int32_t x, int32_t y, int32_t radius, int32_t intensity);
light_t* newLight(int32_t x, int32_t y, int32_t radius, int32_t intensity);
