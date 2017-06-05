/*-------------------------------------------------------------------------------

	BARONY
	File: colors.hpp
	Desc: I can see the rainbow.

	Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
	See LICENSE for details.

-------------------------------------------------------------------------------*/

#pragma once

/*
 * SDL_Color colors.
 */
static const SDL_Color sdlColorWhite = { 255, 255, 255, 255 };


/*
 * The following functions generate the corresponding color uint32_ts from the given surface.
 */
inline uint32_t uint32ColorWhite(const SDL_Surface& surface)
{
	return SDL_MapRGB(surface.format, 255, 255, 255);
}

inline uint32_t uint32ColorGray(const SDL_Surface& surface)
{
	return SDL_MapRGB(surface.format, 127, 127, 127);
}

inline uint32_t uint32ColorBlue(const SDL_Surface& surface)
{
	return SDL_MapRGBA(surface.format, 0, 92, 255, 255);
}

inline uint32_t uint32ColorLightBlue(const SDL_Surface& surface)
{
	return SDL_MapRGBA(surface.format, 0, 255, 255, 255);
}

inline uint32_t uint32ColorBaronyBlue(const SDL_Surface& surface)
{
	return SDL_MapRGBA(surface.format, 0, 192, 255, 255); //Dodger Blue. Apparently.
}

inline uint32_t uint32ColorRed(const SDL_Surface& surface)
{
	return SDL_MapRGBA(surface.format, 255, 0, 0, 255);
}

inline uint32_t uint32ColorGreen(const SDL_Surface& surface)
{
	return SDL_MapRGBA(surface.format, 0, 255, 0, 255);
}

