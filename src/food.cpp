#include "food.hpp"

Food::Food(){
	active = false;
	x = 0.; y = 0.; sx = 0.; sy = 0.;
	rad = 0.; energy = 0.;
	age = 0; isFood = false;
}

Food::~Food(){
}

void Food::Spawn(float x, float y, float energy, float waste, float dir, SimulationParams *params){
	this->x = x;
	this->y = y;
	this->energy = energy + waste;
	float speed = params->foodSpawnMomentum/(this->energy+1.);
	this->sx = cos(dir)*speed;
	this->sy = sin(dir)*speed;
	this->rad = sqrt(this->energy/M_PI);
	this->age = (uint32_t)((energy/this->energy) * params->foodConvertAge);
	this->isFood = (this->age >= params->foodConvertAge);
}

void Food::Frame(SimulationParams *params){
	if(age>=params->foodConvertAge)isFood = true;
	age++;
	float speed = sqrt( pow(sx,2) + pow(sy,2) );
	float ang = atan2( sy , sx );
	speed *= params->foodFriction;
	x = x + sx;
	y = y + sy;
	if((pow(x,2) + pow(y,2)) > pow(params->simRadius-rad,2)){
		float a = atan2(y,x);
		x = cos(a)*(params->simRadius-rad);
		y = sin(a)*(params->simRadius-rad);
		float dc = cos(a-ang);
		if(dc>0.)ang = 2*a-ang+M_PI;
		speed *= params->foodElastic;
	}
	sx = cos(ang)*speed;
	sy = sin(ang)*speed;
}

void Food::Interact(Food *food, SimulationParams *params){
	float rd = rad + food->rad;
	if(x+rd<food->x)return;
	if(x-rd>food->x)return;
	if(y+rd<food->y)return;
	if(y-rd>food->y)return;
	float ds = pow(food->x-x,2) + pow(food->y-y,2);
	if(ds<rd*rd){

		Food *A = this;
		Food *B = food;

		//speed
		float Adir = atan2(A->sy,A->sx);
		float Amag = sqrt(pow(A->sx,2)+pow(A->sy,2));
		float Bdir = atan2(B->sy,B->sx);
		float Bmag = sqrt(pow(B->sx,2)+pow(B->sy,2));

		//collision angle
		float Acol = atan2((B->y)-(A->y),(B->x)-(A->x));
		float Bcol = Acol + M_PI; //atan2((A->y)-(B->y),(A->x)-(B->x));

		//speed towards collision
		float Atdir = Acol;
		float Atmag = cos(Atdir-Adir)*Amag;
		float Btdir = Bcol;
		float Btmag = cos(Btdir-Bdir)*Bmag;

		//energy
		float Amass = M_PI*pow(A->rad,2);
		float Bmass = M_PI*pow(B->rad,2);
		float mass = Amass+Bmass;

		if((Atmag>0.)||(Btmag>0.)){

			//speed away from collision
			float Aax = A->sx - (cos(Atdir)*Atmag);
			float Aay = A->sy - (sin(Atdir)*Atmag);
			float Bax = B->sx - (cos(Btdir)*Btmag);
			float Bay = B->sy - (sin(Btdir)*Btmag);

			//collision
			Btmag = -Btmag;
			float Af = ( params->foodElastic*Bmass*(Btmag-Atmag) + Amass*Atmag + Bmass*Btmag )/mass;
			float Bf = ( params->foodElastic*Amass*(Atmag-Btmag) + Amass*Atmag + Bmass*Btmag )/mass;
			Bf = -Bf;

			//change speed
			A->sx = Aax + cos(Acol)*Af;
			A->sy = Aay + sin(Acol)*Af;
			B->sx = Bax + cos(Bcol)*Bf;
			B->sy = Bay + sin(Bcol)*Bf;

		}

		//move outside
		float dis = sqrt(ds);
		float Ar = Bmass/mass;
		float Br = Amass/mass;
		float m = rd-dis;
		A->x+=cos(Bcol)*m*Ar;
		A->y+=sin(Bcol)*m*Ar;
		B->x+=cos(Acol)*m*Br;
		B->y+=sin(Acol)*m*Br;

	}
}

bool Food::Join(Food *food, SimulationParams *params){
	float rd = rad + food->rad;
	if(x+rd<food->x)return false;
	if(x-rd>food->x)return false;
	if(y+rd<food->y)return false;
	if(y-rd>food->y)return false;
	float ds = pow(food->x-x,2) + pow(food->y-y,2);
	if(ds<rd*rd){
		age = (uint32_t)((age*energy + food->age*food->energy)/(food->energy + energy));
		isFood = (age >= params->foodConvertAge);
		float r = food->energy/(food->energy + energy);
		x += (food->x-x)*r;
		y += (food->y-y)*r;
		energy += food->energy;
		rad = sqrt(energy/M_PI);
		food->energy = 0.;
		food->rad = 0.;
		return true;
	}
	return false;
}

bool Food::Save(unsigned int n){
	char filename[40];
	sprintf(filename,"save/%u.food",n);
	FILE *file = fopen(filename,"wb");
	if(file==NULL)return false;
	if(fwrite((void*)(&x),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&y),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&sx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&sy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&rad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&age),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&isFood),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&active),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	fclose(file);
	return true;
}

bool Food::Load(unsigned int n){
	char filename[40];
	sprintf(filename,"save/%u.food",n);
	FILE *file = fopen(filename,"rb");
	if(file==NULL)return false;
	if(fread((void*)(&x),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&y),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&sx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&sy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&rad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&age),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&isFood),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&active),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	fclose(file);
	return true;
}

void Food::Draw(SDL_Surface *dst, MovableView *view, SimulationParams *params){
	Uint32 cFrom = 0x814A1AFF;
	Uint32 cTo = 0xA7CC70FF;
	Uint32 cFood = 0xCCF888FF;
	if(isFood){
		view->filledCircle(dst,x,y,rad,cFood);
	}else{
		float conv = (float)age/params->foodConvertAge; if(conv>1.)conv=1.;
		SDL_Color from = Uint32ToColor(cFrom);
		SDL_Color to = Uint32ToColor(cTo);
		Uint8 r,g,b,a;
		r = from.r + (int)((((float)to.r)-((float)from.r))*conv );
		g = from.g + (int)((((float)to.g)-((float)from.g))*conv );
		b = from.b + (int)((((float)to.b)-((float)from.b))*conv );
		a = from.unused + (int)((((float)to.unused)-((float)from.unused))*conv );
		view->filledCircle(dst,x,y,rad,RGBA(r,g,b,a));
	}
	view->circle(dst,x,y,rad,0x000000FF);
}
