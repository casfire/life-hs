#include "CasfireSplash.hpp"

#if _CASFIRESPLASH_VER_ != 204
	#warning Invalid CasfireSplash header version
#endif

CasfireSplash::CasfireSplash(){
	timeIn = 1200; timeOut = 400; timeInMin = 0; timeOutMin = 0; ticksInStart = 0; ticksOutStart = 0; ticksLastFrame = 0;
	backSurface = NULL; backIsGenerated = false; backColor = 0xFFFFFFFF; midX = 0.; midY = 0.;
	circles.clear(); particles.clear(); pixels.clear(); logo = NULL; aaLogo = NULL;
}

CasfireSplash::~CasfireSplash(){
	Free();
}

bool CasfireSplash::Generate(Uint16 screenWidth, Uint16 screenHeight){
	Free();
	bool success = true;
	if(!GenerateBackground(screenWidth,screenHeight))success = false;
	GenerateBackCircles(screenWidth,screenHeight);
	if(!GenerateLogo((screenWidth > 800)?600:screenWidth*0.75))success = false;
	return success;
}

void CasfireSplash::FrameIn(SDL_Surface *destination, const char *text, Uint32 textColor){
	if(logo == NULL)return;
	Uint32 now = SDL_GetTicks();
	if(ticksInStart == 0)ticksInStart = now;
	Uint32 lapsed = now - ticksInStart;
	Uint32 timeMS = now - ticksLastFrame; if(timeMS > 200)timeMS = 200;
	ticksLastFrame = now;
	BlitBackground(destination,timeMS);
	if(lapsed < timeIn){
		float p = lapsed/(float)timeIn;
		size_t particlesSize = particles.size();
		if(particlesSize > 0){
			float t = sin(p * M_PI_2);
			float k = (p-0.85)/0.15; if(k<0.)k=0.;
			Uint16 scrMidX = (destination->w/2) - (logo->w/2);
			Uint16 scrMidY = (destination->h/2) - (logo->h/2);
			if(k>0.)BlitLogo(destination,k*0xFF,text,textColor);
			for(size_t n = 0; n<particlesSize; n++){
				Uint16 x = scrMidX + particles[n].outX + (particles[n].picX - particles[n].outX)*t;
				Uint16 y = scrMidY + particles[n].outY + (particles[n].picY - particles[n].outY)*t;
				Uint8 alpha = (Uint8)(p*(((Uint8*)&particles[n].color)[0]));
				pixelRGBA(screen,x,y,((Uint8*)&particles[n].color)[3],((Uint8*)&particles[n].color)[2],((Uint8*)&particles[n].color)[1],alpha);
			}
		}else BlitLogo(destination,p*0xFF,text,textColor);
	}else{
		BlitLogo(destination,0xFF,text,textColor);
	}
}

void CasfireSplash::FrameOut(SDL_Surface *destination){
	if(logo == NULL)return;
	Uint32 now = SDL_GetTicks();
	if(ticksOutStart == 0)ticksOutStart = now;
	Uint32 lapsed = now - ticksOutStart;
	if(lapsed >= timeOut){ Free(); return; }
	ticksLastFrame = now;
	float time = 1.-(lapsed/(float)timeOut);
	BlitLogo(destination,time*0xFF,NULL,0x000000FF);
}

bool CasfireSplash::AnimateIn(SDL_Surface *dst, const char *text, Uint32 textColor){
	bool run = true, interrupted = false; SDL_Event SDLEvent;
	Uint32 start = SDL_GetTicks();
	ticksInStart = start;
	while(run){
		Uint32 now = SDL_GetTicks();
		Uint32 lapsed = now - start;
		while(SDL_PollEvent(&SDLEvent)){
			switch(SDLEvent.type){
			case SDL_QUIT: interrupted = true; break;
			//case SDL_VIDEORESIZE: interrupted = true; break;
			case SDL_KEYDOWN: interrupted = true; break;
			}
		}
		FrameIn(dst,text,textColor);
		SDL_Flip(dst);
		if(lapsed >= timeInMin){
			if(interrupted)run = false;
			else if(lapsed >= timeIn)run = false;
		}
	}
	return !interrupted;
}

bool CasfireSplash::AnimateOut(SDL_Surface *dst){
	bool run = true, interrupted = false; SDL_Event SDLEvent;
	Uint32 start = SDL_GetTicks(), last = start;
	ticksOutStart = start;
	while(run){
		Uint32 now = SDL_GetTicks();
		Uint32 lapsed = now - start;
		Uint32 timeMS = last - now; last = now;
		while(SDL_PollEvent(&SDLEvent)){
			switch(SDLEvent.type){
			case SDL_QUIT: interrupted = true; break;
			//case SDL_VIDEORESIZE: interrupted = true; break;
			case SDL_KEYDOWN: interrupted = true; break;
			}
		}
		BlitBackground(dst,timeMS);
		FrameOut(dst);
		SDL_Flip(dst);
		if(lapsed >= timeOutMin){
			if(interrupted)run = false;
			else if(lapsed >= timeOut)run = false;
		}
	}
	return !interrupted;
}

void CasfireSplash::Free(){
	FreeBackground();
	FreeLogo();
	circles.clear(); particles.clear(); pixels.clear();
}

void CasfireSplash::SetTimings(Uint32 inTime, Uint32 outTime, Uint32 inTimeMin, Uint32 outTimeMin){
	this->timeIn = inTime; this->timeOut = outTime; this->timeInMin = inTimeMin; this->timeOutMin = outTimeMin;
}

bool CasfireSplash::GenerateBackground(Uint16 screenWidth, Uint16 screenHeight, Uint32 color, Uint32 gradientColor, Uint8 random){
	FreeBackground();
	backColor = color;
	backSurface = SDLNew(screenWidth,screenHeight,false);
	if(backSurface == NULL)return false;
	backIsGenerated = true;
	SDL_Color clrBack = Uint32ToColor(color);
	SDL_Color clrGradient = Uint32ToColor(gradientColor);
	midX = xrand(0,screenWidth);
	midY = xrand(0,screenHeight);
	float pm = dis(0,0,screenWidth,screenHeight);
	for(int x = 0; x<screenWidth; x++)for(int y = 0; y<screenHeight; y++){
		float rat = dis(x,y,midX,midY)/pm;
		Uint8 r = clrBack.r + (clrGradient.r - clrBack.r) * rat;
		Uint8 g = clrBack.g + (clrGradient.g - clrBack.g) * rat;
		Uint8 b = clrBack.b + (clrGradient.b - clrBack.b) * rat;
		if((r<=255-random )&&(r>=random ))r += (rand()%(random*2)) - random;
		if((g<=255-random )&&(g>=random ))g += (rand()%(random*2)) - random;
		if((b<=255-random )&&(b>=random ))b += (rand()%(random*2)) - random;
		pixelRGBA(backSurface,x,y,r,g,b,0xFF);
	}
	return true;
}

void CasfireSplash::SetBackground(Uint32 color, SDL_Surface *background){
	FreeBackground();
	backSurface = background;
	backIsGenerated = false;
	backColor = color;
}

void CasfireSplash::GenerateBackCircles(Uint16 screenWidth, Uint16 screenHeight, Uint16 minCount, Uint16 maxCount, Uint16 minSize, Uint16 maxSize, float minSpeed, float maxSpeed, Uint8 minAlpha, Uint8 maxAlpha){
	circles.clear();
	Uint16 count = minCount + (rand()%(maxCount - minCount));
	circles.resize(count);
	float pm = dis(0,0,screenWidth,screenHeight);
	for(Uint16 n = 0; n<count; n++){
		circles[n].size = xrand(minSize,maxSize);
		for(int m = 0; m<100; m++){
			bool found = false;
			circles[n].x = xrand(0,screenWidth);
			circles[n].y = xrand(0,screenHeight);
			for(Uint16 k = 0; k<n; k++){
				if(dis(circles[n].x,circles[n].y,circles[k].x,circles[k].y) * 2.0<circles[n].size+circles[k].size){ found = true; break; }
			}
			if(!found)break;
		}
		circles[n].speed = minSpeed+((1.-(dis(circles[n].x,circles[n].y,midX,midY)/pm))*(maxSpeed - minSpeed));
		if(maxSize == minSize)circles[n].a = maxAlpha;
		else circles[n].a = minAlpha + ((1.-((circles[n].size - minSize)/(maxSize - minSize)))*(maxAlpha - minAlpha));
	}
}

bool CasfireSplash::GenerateLogo(Uint16 width, Uint32 color, bool useParticles, Uint8 particleSkip, Uint8 aaSize){
	FreeLogo();
	Uint16 sizeX = width, sizeY = width/2;
	logo = SDLNew(sizeX,sizeY,false);
	if(logo == NULL)return false;
	aaLogo = SDLNew(sizeX,sizeY,true);
	if(aaLogo == NULL){ SDL_FreeSurface(logo); logo = NULL; return false; }
	Uint32 transColor = (~color)|0x000000FF;
	boxColor(logo,0,0,logo->w,logo->h,transColor);
	boxColor(aaLogo,0,0,aaLogo->w,aaLogo->h,0x00000000);
	SDL_Color colorCopy = Uint32ToColor(color), trueColor = colorCopy;
	Uint8 skip = 0;
	float aa = 1./(1+aaSize); Uint16 total = aaSize*aaSize, times;
	for(Uint16 ny = 0; ny<sizeY; ny++)for(Uint16 nx = 0; nx<sizeX; nx++){
		#define LX(a) ((((a)/(float)sizeX)-.5)*32.)
		#define LY(a) ((((a)/(float)sizeY)-.5)*16.)
		bool onA = IsInside(LX(nx),LY(ny));
		bool onB = IsInside(LX(nx+1.),LY(ny+1.));
		bool onC = IsInside(LX(nx+1.),LY(ny));
		bool onD = IsInside(LX(nx),LY(ny+1.));
		if(onA||onB||onC||onD){
			if(onA&&onB&&onC&&onD){
				pixelColor(logo,nx,ny,color);
				pixelColor(aaLogo,nx,ny,color);
				trueColor.unused = 0xFF;
			}else{
				times = 0;
				for(Uint16 ax = 0; ax<aaSize; ax++)for(Uint16 ay = 0; ay<aaSize; ay++){
					if(IsInside(LX(nx+(ax*aa)),LY(ny+(ay*aa))))times++;
				}
				trueColor.unused = (times/(float)total)*colorCopy.unused;
				pixelRGBA(aaLogo,nx,ny,trueColor.r,trueColor.g,trueColor.b,trueColor.unused);
				pixels.push_back(AAPixel());
				pixels.back().x = nx;
				pixels.back().y = ny;
				pixels.back().r = trueColor.r;
				pixels.back().g = trueColor.g;
				pixels.back().b = trueColor.b;
				pixels.back().a = trueColor.unused;
			}
			if(useParticles&&(skip == 0)){
				particles.push_back(Particle());
				particles.back().picX = nx+.5;
				particles.back().picY = ny+.5;
				float x = LX(nx+0.5), y = LY(ny+0.5);
				float d = (dis(x,y,(x<0.)?-8:8,0)/8.)*M_PI;
				float dir = atan2(y,x+((x<0.)?-8:8)) + xrand(-d,d);
				float m = xrand(0,sizeX);
				particles.back().outX = particles.back().picX + cos(dir)*m;
				particles.back().outY = particles.back().picY + sin(dir)*m;
				particles.back().color = ColorToUint32(trueColor);
				skip++;
			}else{
				skip++;
				if(skip >= particleSkip)skip = 0;
			}
		}
	}
	SDL_SetColorKey(logo,SDL_SRCCOLORKEY,SDL_MapRGB(logo->format,((Uint8*)&transColor)[3],((Uint8*)&transColor)[2],((Uint8*)&transColor)[1]));
	return true;
}

void CasfireSplash::BlitBackground(SDL_Surface *destination, Uint32 timeMS){
	if(backSurface == NULL)boxColor(destination,0,0,destination->w,destination->h,backColor);
	else{
		int x = ((destination->w)/2)-(backSurface->w/2); if(x<0)x=0;
		int y = ((destination->h)/2)-(backSurface->h/2); if(y<0)y=0;
		SDLBlit(backSurface,destination,x,y);
		if(y>0){
			boxColor(destination,0,0,destination->w,y,backColor);
			boxColor(destination,0,destination->h-y,destination->w,destination->h,backColor);
		}
		if(x>0){
			boxColor(destination,0,y,x,destination->h-y,backColor);
			boxColor(destination,destination->w-x,y,destination->w,destination->h-y,backColor);
		}
	}
	SDL_Color c = Uint32ToColor(backColor);
	for(size_t n = 0; n<circles.size(); n++){
		filledCircleRGBA(destination,circles[n].x,circles[n].y,circles[n].size,c.r,c.g,c.b,circles[n].a);
		float a = atan2(circles[n].y-midY,circles[n].x-midX)+M_PI/2.;
		circles[n].x += cos(a)*circles[n].speed*timeMS;
		circles[n].y += sin(a)*circles[n].speed*timeMS;
	}
}

void CasfireSplash::BlitLogo(SDL_Surface *destination, Uint8 alpha, const char *text, Uint32 textColor){
	int lh = 0;
	if(alpha == 0xFF){
		if(aaLogo != NULL){
			SDLBlit(aaLogo,destination,(destination->w/2)-(aaLogo->w/2),(destination->h/2)-(aaLogo->h/2));
			lh = aaLogo->h/2;
		}
	}else{
		if(logo != NULL){
			SDL_SetAlpha(logo,SDL_SRCALPHA,alpha);
			Uint16 x = (destination->w/2)-(logo->w/2), y = (destination->h/2)-(logo->h/2);
			float a = (alpha/(float)0xFF);
			SDLBlit(logo,destination,x,y);
			lh = logo->h/2;
			for(size_t n = 0; n<pixels.size(); n++){
				pixelRGBA(destination,x+pixels[n].x,y+pixels[n].y,pixels[n].r,pixels[n].g,pixels[n].b,pixels[n].a*a);
			}
		}
	}
	if(text != NULL)stringColor(destination,(destination->w/2)-(strlen(text)*4),(destination->h/2)+lh,text,textColor);
}

void CasfireSplash::FreeBackground(){
	if(backIsGenerated&&(backSurface != NULL)){
		SDL_FreeSurface(backSurface);
		backSurface = NULL;
	}
	backIsGenerated = false;
}

void CasfireSplash::FreeLogo(){
	if(logo != NULL){
		SDL_FreeSurface(logo);
		logo = NULL;
	}
	if(aaLogo != NULL){
		SDL_FreeSurface(aaLogo);
		aaLogo = NULL;
	}
	pixels.clear();
}

float CasfireSplash::xrand(float min, float max){
	return min+((float(rand())/RAND_MAX)*(max-min));
}

float CasfireSplash::dis(float ax, float ay, float bx, float by){
	return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}

bool CasfireSplash::IsInside(float x, float y){
	if(x<0.){
		if((x>=-8.)&&(y>=-1.)&&(y<=1.))return true;
		else{
			float d = dis(x,y,-8.,0.);
			if(d<=2)return true;
			else if((d<=6.)&&(d>=4.)){
				float a = atan2(y,x+8.);
				if((a>M_PI/4.)||(a<M_PI/-4.))return true;
			}
		}
	}else{
		if((x<=8.)&&(y>=-1.)&&(y<=1.))return true;
		else{
			float d = dis(x,y,8.,0.);
			if(d<=2.)return true;
			else if((d<=6.)&&(d>=4.)){
				float a = atan2(y,x-8.);
				if((a<M_PI*3./4.)&&(a>M_PI*-3./4.))return true;
			}
		}
	}
	return false;
}
