#pragma once
#include <SDL.h>
#include "entity.hpp"
#include "maps.hpp"

#define FLIP_VERTICAL 1
#define FLIP_HORIZONTAL 2
SDL_Surface* flipSurface(SDL_Surface* surface, int flags);
void drawCircle(int x, int y, float radius, uint32_t color, Uint8 alpha);
void drawArc(int x, int y, float radius, float angle1, float angle2, uint32_t color, Uint8 alpha);
void drawLine(int x1, int y1, int x2, int y2, uint32_t color, Uint8 alpha);
int drawRect(SDL_Rect* src, uint32_t color, Uint8 alpha);
int drawBox(SDL_Rect* src, uint32_t color, Uint8 alpha);
void drawGear(Sint16 x, Sint16 y, float size, int32_t rotation);
void drawImage(SDL_Surface* image, SDL_Rect* src, SDL_Rect* pos);
void drawImageScaled(SDL_Surface* image, SDL_Rect* src, SDL_Rect* pos);
void drawImageAlpha(SDL_Surface* image, SDL_Rect* src, SDL_Rect* pos, Uint8 alpha);
void drawImageColor(SDL_Surface* image, SDL_Rect* src, SDL_Rect* pos, uint32_t color);
void drawImageFancy(SDL_Surface* image, uint32_t color, float angle, SDL_Rect* src, SDL_Rect* pos);
void drawImageRotatedAlpha(SDL_Surface* image, SDL_Rect* src, SDL_Rect* pos, float angle, Uint8 alpha);
SDL_Surface* scaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height);
void drawSky3D(view_t* camera, SDL_Surface* tex);
void drawLayer(long camx, long camy, int z, map_t* map);
void drawBackground(long camx, long camy);
void drawForeground(long camx, long camy);
void drawClearBuffers();
void raycast(view_t* camera, int mode);
void drawFloors(view_t* camera);
void drawSky(SDL_Surface* srfc);
void drawVoxel(view_t* camera, Entity* entity);
void drawEntities3D(view_t* camera, int mode);
void drawPalette(voxel_t* model);
void drawEntities2D(long camx, long camy);
void drawGrid(long camx, long camy);
void drawEditormap(long camx, long camy);
void drawWindow(int x1, int y1, int x2, int y2);
void drawDepressed(int x1, int y1, int x2, int y2);
void drawWindowFancy(int x1, int y1, int x2, int y2);
SDL_Rect ttfPrintTextColor( TTF_Font* font, int x, int y, uint32_t color, bool outline, const char* str );
SDL_Rect ttfPrintText( TTF_Font* font, int x, int y, const char* str );
SDL_Rect ttfPrintTextFormattedColor( TTF_Font* font, int x, int y, uint32_t color, char* fmt, ... );
SDL_Rect ttfPrintTextFormatted( TTF_Font* font, int x, int y, char* fmt, ... );
void printTextFormatted( SDL_Surface* font_bmp, int x, int y, char* fmt, ... );
void printTextFormattedAlpha(SDL_Surface* font_bmp, int x, int y, Uint8 alpha, char* fmt, ...);
void printTextFormattedColor(SDL_Surface* font_bmp, int x, int y, uint32_t color, char* fmt, ...);
void printTextFormattedFancy(SDL_Surface* font_bmp, int x, int y, uint32_t color, float angle, float scale, char* fmt, ...);
void printText( SDL_Surface* font_bmp, int x, int y, char* str );
void drawSprite(view_t* camera, Entity* entity);
void drawTooltip(SDL_Rect* src);

typedef struct view_t
{
	float x, y, z;
	float ang;
	float vang;
	int32_t winx, winy, winw, winh;
} view_t;

extern view_t camera;

