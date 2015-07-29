/* MovableView v1.1.3 Date: 2012-04-20 */

#pragma once
#ifndef _MOVABLEVIEW_HPP_
#define _MOVABLEVIEW_HPP_
#define _MOVABLEVIEW_VER_ 113

#include "SDL_gfxPrimitives.h"

class MovableView{
	float tPosX, tPosY;
	float tOffX, tOffY;
	float tZoom, tRot;
	bool tRevX, tRevY;
	Uint8 dragMask, dragMaskOther;
	bool eventCanZoom; float eventZoomAmount;
	float dragX, dragY, dragOffX, dragOffY;
	bool useAA;
public:
	MovableView();
	bool Event(SDL_Event *event, int posX1 = 0, int posY1 = 0, int posX2 = 0, int posY2 = 0, int offX = 0, int offY = 0);

	void SetMiddle(float x, float y);

	void SetOff(float offX, float offY);
	void SetOffX(float offX);
	void SetOffY(float offY);
	void SetZoom(float zoom);
	void SetRot(float rotation);
	void SetUseAA(bool useAntiAliasing);
	void SetReverse(bool reverseX = false, bool reverseY = true);
	void SetReverseX(bool reverseX = false);
	void SetReverseY(bool reverseY = true);
	float GetOffX();
	float GetOffY();
	float GetZoom();
	float GetRot();
	bool GetUseAA();
	bool GetReverseX();
	bool GetReverseY();

	void SetCanZoom(bool canZoom);
	void SetZoomAmount(float amount);
	void SetCanDrag(Uint8 SDLButton, bool enable); //set particular SDL_BUTTON to true/false
	void SetDragMask(Uint8 SDLButtonMask = SDL_BUTTON(SDL_BUTTON_LEFT)|SDL_BUTTON(SDL_BUTTON_RIGHT)); //set mask of all buttons - 0 to disable
	bool GetCanZoom();
	bool IsDrag(Uint8 button);

	void GetCoords(float x, float y, int *outX, int *outY);
	void GetCoords(float x, float y, Sint16 *outX, Sint16 *outY);
	void GetRevCoords(int x, int y, float *outX, float *outY);

	void pixel(SDL_Surface *dst, float x, float y, Uint32 color);
	void line(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color);
	void rectangle(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color);
	void box(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color);
	void circle(SDL_Surface *dst, float x, float y, float rad, Uint32 color);
	void filledCircle(SDL_Surface *dst, float x, float y, float rad, Uint32 color);
	void ellipse(SDL_Surface *dst, float x, float y, float rx, float ry, Uint32 color);
	void arc(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color); //in radians
	void pie(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color); //in radians
	void filledPie(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color); //in radians
	void trigon(SDL_Surface *dst, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color);
	void filledTrigon(SDL_Surface *dst, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color);
	void polygon(SDL_Surface *dst, const float *vx, const float *vy, int n, Uint32 color);
	void filledPolygon(SDL_Surface *dst, const float *vx, const float *vy, int n, Uint32 color);
};

#endif
