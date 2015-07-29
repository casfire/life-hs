#include "MovableView.hpp"

#if _MOVABLEVIEW_VER_ != 113
	#warning Invalid MovableView header version
#endif

MovableView::MovableView(){
	tPosX = 0.; tPosY = 0.;
	tOffX = 0.; tOffY = 0.;
	tZoom = 1.; tRot = 0.;
	tRevX = false; tRevY = true;
	dragMask = SDL_BUTTON(SDL_BUTTON_LEFT)|SDL_BUTTON(SDL_BUTTON_RIGHT);
	dragMaskOther = 0;
	eventCanZoom = true; eventZoomAmount = 0.9;
	dragX = 0.; dragY = 0.; dragOffX = 0.; dragOffY = 0.;
	useAA = true;
}

bool MovableView::Event(SDL_Event *event, int posX1, int posY1, int posX2, int posY2, int offX, int offY){
	int mx,my;
	switch(event->type){
	case SDL_MOUSEBUTTONDOWN:
		mx = event->button.x+offX;
		my = event->button.y+offY;
		if(((posX1==posX2)||((mx>posX1)&&(mx<posX2))) && ((posY1==posY2)||((my>posY1)&&(my<posY2)))){
			dragMaskOther |= SDL_BUTTON(event->button.button);
			if(dragMask&SDL_BUTTON(event->button.button)){
				GetRevCoords(mx,my,&dragX,&dragY);
				dragOffX = tOffX;
				dragOffY = tOffY;
				return true;
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if(event->button.button == SDL_BUTTON_WHEELUP){
			mx = event->button.x+offX;
			my = event->button.y+offY;
			if(eventCanZoom)if(((posX1==posX2)||((mx>posX1)&&(mx<posX2)))&&((posY1==posY2)||((my>posY1)&&(my<posY2)))){
				tZoom /= eventZoomAmount;
				return true;
			}
		}else if(event->button.button == SDL_BUTTON_WHEELDOWN){
			mx = event->button.x+offX;
			my = event->button.y+offY;
			if(eventCanZoom)if(((posX1==posX2)||((mx>posX1)&&(mx<posX2)))&&((posY1==posY2)||((my>posY1)&&(my<posY2)))){
				tZoom *= eventZoomAmount;
				return true;
			}
		}
		dragMaskOther &= ~SDL_BUTTON(event->button.button);
		break;
	case SDL_MOUSEMOTION:
		if(dragMask&dragMaskOther){
			mx = event->motion.x+offX;
			my = event->motion.y+offY;
			float mX = 0, mY = 0;
			tOffX = dragOffX;
			tOffY = dragOffY;
			GetRevCoords(mx,my,&mX,&mY);
			tOffX = dragOffX + mX - dragX;
			tOffY = dragOffY + mY - dragY;
		}
		break;
	}
	return false;
}

void MovableView::SetMiddle(float x, float y){
	tPosX = x;
	tPosY = y;
}

void MovableView::SetOff(float offX, float offY){
	tOffX = offX;
	tOffY = offY;
}

void MovableView::SetOffX(float offX){
	tOffX = offX;
}

void MovableView::SetOffY(float offY){
	tOffY = offY;
}

void MovableView::SetZoom(float zoom){
	tZoom = zoom;
}

void MovableView::SetRot(float rotation){
	tRot = rotation;
}

void MovableView::SetUseAA(bool useAntiAliasing){
	useAA = useAntiAliasing;
}

void MovableView::SetReverse(bool reverseX, bool reverseY){
	tRevX = reverseX;
	tRevY = reverseY;
}

void MovableView::SetReverseX(bool reverseX){
	tRevX = reverseX;
}

void MovableView::SetReverseY(bool reverseY){
	tRevY = reverseY;
}

float MovableView::GetOffX(){
	return tOffX;
}

float MovableView::GetOffY(){
	return tOffY;
}

float MovableView::GetZoom(){
	return tZoom;
}

float MovableView::GetRot(){
	return tRot;
}

bool MovableView::GetUseAA(){
	return useAA;
}

bool MovableView::GetReverseX(){
	return tRevX;
}

bool MovableView::GetReverseY(){
	return tRevY;
}

void MovableView::SetCanZoom(bool canZoom){
	eventCanZoom = canZoom;
}

void MovableView::SetZoomAmount(float amount){
	eventZoomAmount = amount;
}

void MovableView::SetCanDrag(Uint8 SDLButton, bool enable){
	if(enable){
		dragMask |= SDL_BUTTON(SDLButton);
	}else{
		dragMask &= ~SDL_BUTTON(SDLButton);
	}
}

void MovableView::SetDragMask(Uint8 SDLButtonMask){
	dragMask = SDLButtonMask;
}

bool MovableView::GetCanZoom(){
	return eventCanZoom;
}

bool MovableView::IsDrag(Uint8 button){
	return (dragMaskOther&SDL_BUTTON(button));
}

void MovableView::GetCoords(float x, float y, int *outX, int *outY){
	float tx = (x+tOffX)*tZoom;
	float ty = (y+tOffY)*tZoom;
	float r = atan2(ty,tx)-tRot;
	float d = sqrt((tx*tx)+(ty*ty));
	tx = cos(r)*d;
	ty = sin(r)*d;
	if(tRevX)tx = -tx;
	if(tRevY)ty = -ty;
	tx = tPosX+tx;
	ty = tPosY+ty;
	*outX = (int)(tx);
	*outY = (int)(ty);
}

void MovableView::GetCoords(float x, float y, Sint16 *outX, Sint16 *outY){
	float tx = (x+tOffX)*tZoom;
	float ty = (y+tOffY)*tZoom;
	float r = atan2(ty,tx)-tRot;
	float d = sqrt((tx*tx)+(ty*ty));
	tx = cos(r)*d;
	ty = sin(r)*d;
	if(tRevX)tx = -tx;
	if(tRevY)ty = -ty;
	tx = tPosX+tx; //tx = tPosX+(tx*tStretchX);
	ty = tPosY+ty; //ty = tPosY+(ty*tStretchY);
	*outX = (Sint16)(tx);
	*outY = (Sint16)(ty);
}

void MovableView::GetRevCoords(int x, int y, float *outX, float *outY){
	float tx = ((float)x)-tPosX; //float tx = (((float)x)-tPosX)/tStretchX;
	float ty = ((float)y)-tPosY; //float ty = (((float)y)-tPosY)/tStretchY;
	if(tRevX)tx = -tx;
	if(tRevY)ty = -ty;
	float r = atan2(ty,tx)+tRot;
	float d = sqrt((tx*tx)+(ty*ty));
	tx = cos(r)*d;
	ty = sin(r)*d;
	*outX = (tx/tZoom)-tOffX;
	*outY = (ty/tZoom)-tOffY;
}

void MovableView::pixel(SDL_Surface *dst, float x, float y, Uint32 color){
	int px, py;
	GetCoords(x,y,&px,&py);
	pixelColor(dst,px,py,color);
}

void MovableView::line(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color){
	int px1, py1, px2, py2;
	GetCoords(x1,y1,&px1,&py1);
	GetCoords(x2,y2,&px2,&py2);
	if(useAA)aalineColor(dst,px1,py1,px2,py2,color);
	else lineColor(dst,px1,py1,px2,py2,color);
}

void MovableView::rectangle(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color){
	int px1, py1, px2, py2, px3, py3, px4, py4;
	GetCoords(x1,y1,&px1,&py1);
	GetCoords(x1,y2,&px2,&py2);
	GetCoords(x2,y2,&px3,&py3);
	GetCoords(x2,y1,&px4,&py4);
	if(useAA){
		aalineColor(dst,px1,py1,px2,py2,color);
		aalineColor(dst,px2,py2,px3,py3,color);
		aalineColor(dst,px3,py3,px4,py4,color);
		aalineColor(dst,px4,py4,px1,py1,color);
	}else{
		lineColor(dst,px1,py1,px2,py2,color);
		lineColor(dst,px2,py2,px3,py3,color);
		lineColor(dst,px3,py3,px4,py4,color);
		lineColor(dst,px4,py4,px1,py1,color);
	}
}

void MovableView::box(SDL_Surface *dst, float x1, float y1, float x2, float y2, Uint32 color){
	Sint16 px[4], py[4];
	int tx, ty;
	GetCoords(x1,y1,&tx,&ty); px[0] = tx; py[0] = ty;
	GetCoords(x1,y2,&tx,&ty); px[1] = tx; py[1] = ty;
	GetCoords(x2,y2,&tx,&ty); px[2] = tx; py[2] = ty;
	GetCoords(x2,y1,&tx,&ty); px[3] = tx; py[3] = ty;
	filledPolygonColor(dst,px,py,4,color);
}

void MovableView::circle(SDL_Surface *dst, float x, float y, float rad, Uint32 color){
	int px, py, pr;
	GetCoords(x,y,&px,&py);
	pr = (int)(rad*tZoom);
	if(useAA&&(pr>2)&&(pr<800))aacircleColor(dst,px,py,pr,color);
	else circleColor(dst,px,py,pr,color);
}

void MovableView::filledCircle(SDL_Surface *dst, float x, float y, float rad, Uint32 color){
	int px, py, pr;
	GetCoords(x,y,&px,&py);
	pr = (int)(rad*tZoom);
	filledCircleColor(dst,px,py,pr,color);
}

#define CIRCLE_KAPPA 0.5522847498
void MovableView::ellipse(SDL_Surface *dst, float x, float y, float rx, float ry, Uint32 color){
	Sint16 px[13], py[13]; int tx, ty;
	float kx = rx * CIRCLE_KAPPA;
	float ky = ry * CIRCLE_KAPPA;
	GetCoords(x,y+ry,&tx,&ty);
	px[0] = tx; py[0] = ty;
	GetCoords(x+kx,y+ry,&tx,&ty);
	px[1] = tx; py[1] = ty;
	GetCoords(x+rx,y+ky,&tx,&ty);
	px[2] = tx; py[2] = ty;
	GetCoords(x+rx,y,&tx,&ty);
	px[3] = tx; py[3] = ty;
	GetCoords(x+rx,y-ky,&tx,&ty);
	px[4] = tx; py[4] = ty;
	GetCoords(x+kx,y-ry,&tx,&ty);
	px[5] = tx; py[5] = ty;
	GetCoords(x,y-ry,&tx,&ty);
	px[6] = tx; py[6] = ty;
	GetCoords(x-kx,y-ry,&tx,&ty);
	px[7] = tx; py[7] = ty;
	GetCoords(x-rx,y-ky,&tx,&ty);
	px[8] = tx; py[8] = ty;
	GetCoords(x-rx,y,&tx,&ty);
	px[9] = tx; py[9] = ty;
	GetCoords(x-rx,y+ky,&tx,&ty);
	px[10] = tx; py[10] = ty;
	GetCoords(x-kx,y+ry,&tx,&ty);
	px[11] = tx; py[11] = ty;
	GetCoords(x,y+ry,&tx,&ty);
	px[12] = tx; py[12]=ty;
	bezierColor(dst,px,py,4,256,color);
	bezierColor(dst,px+3,py+3,4,256,color);
	bezierColor(dst,px+6,py+6,4,256,color);
	bezierColor(dst,px+9,py+9,4,256,color);
}

#define RADTODEG 57.2957795131
void MovableView::arc(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color){
	int px, py, pr, pstart, pend;
	GetCoords(x,y,&px,&py);
	pr = (int)(rad*tZoom);
	pstart = RADTODEG * (-end + tRot);
	pend = RADTODEG * (-start + tRot);
	arcColor(dst,px,py,pr,pstart,pend,color);
}

void MovableView::pie(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color){
	int px, py, pr, pstart, pend;
	GetCoords(x,y,&px,&py);
	pr = (int)(rad*tZoom);
	pstart = RADTODEG * (-end + tRot);
	pend = RADTODEG * (-start + tRot);
	pieColor(dst,px,py,pr,pstart,pend,color);
}

void MovableView::filledPie(SDL_Surface *dst, float x, float y, float rad, float start, float end, Uint32 color){
	int px, py, pr, pstart, pend;
	GetCoords(x,y,&px,&py);
	pr = (int)(rad*tZoom);
	pstart = RADTODEG * (-end + tRot);
	pend = RADTODEG * (-start + tRot);
	filledPieColor(dst,px,py,pr,pstart,pend,color);
}

void MovableView::trigon(SDL_Surface *dst, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color){
	int px1, py1, px2, py2, px3, py3;
	GetCoords(x1,y1,&px1,&py1);
	GetCoords(x2,y2,&px2,&py2);
	GetCoords(x3,y3,&px3,&py3);
	if(useAA)aatrigonColor(dst,px1,py1,px2,py2,px3,py3,color);
	else trigonColor(dst,px1,py1,px2,py2,px3,py3,color);
}

void MovableView::filledTrigon(SDL_Surface *dst, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color){
	int px1, py1, px2, py2, px3, py3;
	GetCoords(x1,y1,&px1,&py1);
	GetCoords(x2,y2,&px2,&py2);
	GetCoords(x3,y3,&px3,&py3);
	filledTrigonColor(dst,px1,py1,px2,py2,px3,py3,color);
}

void MovableView::polygon(SDL_Surface *dst, const float *vx, const float *vy, int n, Uint32 color){
	Sint16 *px = new Sint16[n], *py = new Sint16[n];
	for(int k=0; k<n; k++)GetCoords(vx[k],vy[k],&px[k],&py[k]);
	if(useAA)aapolygonColor(dst,px,py,n,color);
	else polygonColor(dst,px,py,n,color);
	delete[] px;
	delete[] py;
}

void MovableView::filledPolygon(SDL_Surface *dst, const float *vx, const float *vy, int n, Uint32 color){
	Sint16 *px = new Sint16[n], *py = new Sint16[n];
	for(int k=0; k<n; k++)GetCoords(vx[k],vy[k],&px[k],&py[k]);
	filledPolygonColor(dst,px,py,n,color);
	delete[] px;
	delete[] py;
}
