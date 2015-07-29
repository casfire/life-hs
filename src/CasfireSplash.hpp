/* CasfireSplash v2.0.4 Date: 2013-04-20 */

#pragma once
#ifndef _CASFIRESPLASH_HPP_
#define _CASFIRESPLASH_HPP_
#define _CASFIRESPLASH_VER_ 204

#include <math.h>
#include <vector>
#include "SDLHelper.hpp"
#include "SDL_gfxPrimitives.h"

class CasfireSplash{
public:
	CasfireSplash(); ~CasfireSplash();

	//Calls GenerateBackground, GenerateBackCircles and GenerateLogo
	bool Generate(Uint16 screenWidth, Uint16 screenHeight);

	//Draws a single splash screen frame (call this in your own loop, text can change)
	void FrameIn(SDL_Surface *destination, const char *text = NULL, Uint32 textColor = 0x000000FF);
	void FrameOut(SDL_Surface *destination);

	//Animates the splash and halts the program. returns true if it finished and false if it was interrupted
	bool AnimateIn(SDL_Surface *dst, const char *text = NULL, Uint32 textColor = 0x000000FF);
	bool AnimateOut(SDL_Surface *dst);

	//Frees all memory and stops Frame and Animate functions from doing anything
	void Free();

	//Set timings in milliseconds, minimum time is only used by Animate functions
	void SetTimings(Uint32 inTime, Uint32 outTime, Uint32 inTimeMin = 0, Uint32 outTimeMin = 0);

	//Background
	bool GenerateBackground(Uint16 screenWidth, Uint16 screenHeight, Uint32 color = 0xD7E2E6FF, Uint32 gradientColor = 0x32505AFF, Uint8 random = 4);
	void SetBackground(Uint32 color, SDL_Surface *background = NULL);
	void GenerateBackCircles(Uint16 screenWidth, Uint16 screenHeight, Uint16 minCount = 12, Uint16 maxCount = 16, Uint16 minSize = 10, Uint16 maxSize = 60, float minSpeed = 0.00, float maxSpeed = 0.02, Uint8 minAlpha = 0x33, Uint8 maxAlpha = 0x66);

	//Logo
	bool GenerateLogo(Uint16 width, Uint32 color = 0x000000FF, bool useParticles = true, Uint8 particleSkip = 1, Uint8 aaSize = 8);

private:
	struct BackCircle{ float x, y, size, speed; Uint8 a; };
	struct Particle{ float picX, picY, outX, outY; Uint32 color; };
	struct AAPixel{ Uint16 x, y; Uint8 r, g, b, a; };
	Uint32 timeIn, timeOut, timeInMin, timeOutMin, ticksInStart, ticksOutStart, ticksLastFrame;
	SDL_Surface *backSurface; bool backIsGenerated; Uint32 backColor; float midX, midY;
	std::vector<BackCircle> circles;
	std::vector<Particle> particles;
	std::vector<AAPixel> pixels;
	SDL_Surface *logo, *aaLogo;

	void BlitBackground(SDL_Surface *screen, Uint32 timeMS);
	void BlitLogo(SDL_Surface *screen, Uint8 alpha, const char *text, Uint32 textColor);
	void FreeBackground();
	void FreeLogo();
	float xrand(float min, float max);
	float dis(float ax, float ay, float bx, float by);
	bool IsInside(float x, float y);
};

#endif
