#pragma once

typedef struct voxel_t
{
	int32_t sizex, sizey, sizez;
	Uint8* data;
	Uint8 palette[256][3];
} voxel_t;
