#include "simulation.hpp"

#define FOODRESERVE 200
#define CRITRESERVE 100

Simulation::Simulation(){
	Clear();
}

Simulation::~Simulation(){
	food.clear();
	critter.clear();
}

void Simulation::Clear(){
	SetDefaultParams();
	food.clear();
	critter.clear();
	frameCount = 0;
	foodCount = 0;
	critCount = 0;
	critSelect = 0; critSelected = false;
}

void Simulation::SetDefaultParams(){
	params.initialRandomAlg = RNDWELL;
	params.initialRandomSeed = 49;
	params.initialFoodCount = 400;
	params.initialCritCount = 400;
	params.initialFoodDistribution = 0.05;

	memset(&params.rnd,0,sizeof(Random));
	params.rnd.Set(params.initialRandomAlg);
	params.rnd.Srand(params.initialRandomSeed);
	params.simRadius = 60.;
	params.foodFriction = 0.99996;
	params.foodElastic = 1.; //0.96;
	params.foodSpawnMomentum = 0.8;
	params.foodConvertAge = 80;
	params.foodMaxInWorld = 600;
	params.critMinInWorld = 200;
	params.critMaxInWorld = 1000;
	params.critMinGenNetSize = 5;
	params.critMaxGenNetSize = 200;
	params.critMinGenRadius = 0.8;
	params.critMaxGenRadius = 2.0;
	params.critMinGenGrowth = 0.002;
	params.critMaxGenGrowth = 0.1;
	params.critMinGenSpeed = 0.1;
	params.critMaxGenSpeed = 1.0;
	params.critMutateSize = 0.16;
	params.critMutateDiv = 0.96;
	params.critMultiplierSpin = 0.1;
	params.critMultiplierMoveForward = 1.;
	params.critMultiplierMoveBackward = 0.4;
	params.critEnergyBirthFromParents = 0.5;
	params.critMaxProccessedEnergy = 20.;
	params.critMoodThershold = 0.1;
	params.critAttackEnergyTake = 0.08;
	params.critEnergyMinToGiveBirth = 0.2;
	params.critEnergyMinProcessedToGiveBirth = 0.6;
	params.critEnergyToLive = 0.0008;
	params.critEnergyToMoveForw = params.critEnergyToLive * 2.;
	params.critEnergyToMoveBack = params.critEnergyToLive * 4.;
	params.critEnergyToChangeMove = params.critEnergyToLive * 0.5;
	params.critEnergyToSpin = params.critEnergyToLive * 2.;
	params.critEnergyToChangeSpin = params.critEnergyToLive * 0.5;
	params.critEnergyToMoodBad = params.critEnergyToLive * 2.;
	params.critEnergyToMoodGood = params.critEnergyToLive * 0.2;
	params.critEnergyToChangeMood = params.critEnergyToLive * 8.;
	params.critEnergyToExpose = params.critEnergyToLive * 1.;
	params.critEnergyToIncreaseExpose = params.critEnergyToLive * 6.;
	params.critEnergyToDecreaseExpose = params.critEnergyToLive * 0.2;
	params.critEnergyToBrain = params.critEnergyToLive * 0.02;

}

void Simulation::Start(){
	memset(&params.rnd,0,sizeof(Random));
	params.rnd.Set(params.initialRandomAlg);
	params.rnd.Srand(params.initialRandomSeed);
	frameCount = 0;

	for(unsigned int n=0; n<food.size(); n++)food[n].active = false;
	foodCount = 0; food.clear(); food.reserve(params.foodMaxInWorld + FOODRESERVE);
	for(unsigned int n=0; n<critter.size(); n++)critter[n].active = false;
	critCount = 0; critter.clear(); critter.reserve(params.critMaxInWorld + CRITRESERVE);

	float canvasArea = M_PI*pow(params.simRadius,2);
	float energyPerFood = (params.initialFoodDistribution*canvasArea)/params.initialFoodCount;
	for(unsigned int n=0; n<params.initialFoodCount; n++){
		float d = sqrt(params.rnd.xrand(0.,1.))*params.simRadius;
		float a = params.rnd.xrand(-M_PI,M_PI);
		AddFood(cos(a)*d,sin(a)*d,energyPerFood,0.);
	}

	for(unsigned int n=0; n<params.initialCritCount; n++){
		float d = sqrt(params.rnd.xrand(0.,1.))*params.simRadius;
		float a = params.rnd.xrand(-M_PI,M_PI);
		AddCritter(cos(a)*d,sin(a)*d);
	}

}

void Simulation::Frame(){
	frameCount++;

	for(unsigned int n=0; n<food.size(); n++){
		if(!food[n].active)continue;
		for(unsigned int k=0; k<n; k++){
			if(!food[k].active)continue;
			if(params.foodMaxInWorld<foodCount){
				if(food[n].Join(&food[k],&params))RemoveFood(k);
			}else food[n].Interact(&food[k],&params);
		}
		food[n].Frame(&params);
	}

	for(unsigned int n=0; n<critter.size(); n++){
		if(!critter[n].active)continue;
		for(unsigned int k=0; k<food.size(); k++){
			if(!food[k].active)continue;
			if(critter[n].Interact(&food[k],&params))RemoveFood(k);
		}
		if(critter[n].Frame(this)){ RemoveCritter(n); continue; }
		for(unsigned int k=0; k<n; k++){
			if(!critter[k].active)continue;
			critter[n].Interact(&critter[k],this);
		}
	}

}

void Simulation::Draw(SDL_Surface *dst, MovableView *view){
	view->circle(dst,0.,0.,params.simRadius,0x000000FF); //border
	for(uint32_t n=0; n<food.size(); n++)if(food[n].active)food[n].Draw(dst,view,&params);
	for(uint32_t n=0; n<critter.size(); n++)if(critter[n].active)critter[n].Draw(dst,view,((critSelected&&(critSelect==n))?true:false));
}

void Simulation::DrawAxes(SDL_Surface *dst, MovableView *view){
	bool aa = view->GetUseAA();
	view->line(dst,0,-params.simRadius,0,params.simRadius,0x00000088);
	view->line(dst,-params.simRadius,0,params.simRadius,0,0x00000088);
	view->SetUseAA(false);
	int c = (int)params.simRadius;
	if(view->GetZoom()*0.2>2.)for(int n=-c; n<c; n++){
		if(n==0)continue;
		view->line(dst,-0.2,n,0.2,n,0x00000066);
		view->line(dst,n,-0.2,n,0.2,0x00000066);
	}
	view->SetUseAA(aa);
}

bool Simulation::CenterSelected(MovableView *view, bool location, bool rotation){
	if(critSelected){
		if(location)view->SetOff(-critter[critSelect].x, -critter[critSelect].y);
		if(rotation)view->SetRot(critter[critSelect].rot - M_PI/2.);
		return true;
	}
	return false;
}

bool Simulation::IsSelected(){
	return critSelected;
}

void Simulation::AddCritter(float x, float y){
	Critter *c = NULL;
	for(unsigned int n=0; n<critter.size(); n++){
		if(!critter[n].active){
			c = &critter[n];
			break;
		}
	}
	if(c==NULL){
		if(critter.capacity()<=critter.size())return;
		critter.push_back(Critter());
		c = &critter.back();
	}
	if(critCount>params.critMaxInWorld){
		unsigned int t = 0;
		do{ t = params.rnd.Next()%critter.size(); }while(!critter[t].active);
		RemoveCritter(t);
	}
	c->active = true;
	c->Spawn(x,y,&params);
	critCount++;
}

void Simulation::AddCritter(float x, float y, Critter *parentA, Critter *parentB){
	Critter *c = NULL;
	for(unsigned int n=0; n<critter.size(); n++){
		if(!critter[n].active){
			c = &critter[n];
			break;
		}
	}
	if(c==NULL){
		if(critter.capacity()<=critter.size())return;
		critter.push_back(Critter());
		c = &critter.back();
	}
	if(critCount>params.critMaxInWorld){
		unsigned int t = 0;
		do{ t = params.rnd.Next()%critter.size(); }while((!critter[t].active)||(parentA==&critter[t])||(parentB==&critter[t]));
		RemoveCritter(t);
	}
	c->active = true;
	c->Spawn(x,y,parentA,parentB,&params);
	critCount++;
}

void Simulation::RemoveCritter(uint32_t i){
	critCount--;
	critter[i].active = false;
	if(critSelected&&(i==critSelect)){ critSelect = 0; critSelected = false; }
	AddFood(critter[i].x, critter[i].y, critter[i].energy, critter[i].waste, critter[i].rot);
	critter[i].waste = 0.;
	critter[i].energy = 0.;
	if(critCount<params.critMinInWorld){
		float d = sqrt(params.rnd.xrand(0.,1.))*params.simRadius;
		float a = params.rnd.xrand(-M_PI,M_PI);
		Critter *A = NULL, *B = NULL;
		if(critCount>1){
			do{ A = &critter.at(params.rnd.Next()%critter.size()); }while(!(A->active));
			do{ B = &critter.at(params.rnd.Next()%critter.size()); }while(!(B->active));
		}
		if((A!=NULL)&&(B!=NULL))AddCritter(cos(a)*d,sin(a)*d,A,B);
	}
}

void Simulation::AddFood(float x, float y, float energy, float waste){
	AddFood(x,y,energy,waste,params.rnd.xrand(-M_PI,M_PI));
}

void Simulation::AddFood(float x, float y, float energy, float waste, float dir){
	Food *f = NULL;
	for(unsigned int n=0; n<food.size(); n++){
		if(!food[n].active){
			f = &food[n];
			break;
		}
	}
	if(f==NULL){
		if(food.capacity()<=food.size())return;
		food.resize(food.size()+1);
		f = &food.back();
	}
	f->Spawn(x,y,energy,waste,dir,&params);
	f->active = true;
	foodCount++;
}

void Simulation::RemoveFood(uint32_t i){
	foodCount--;
	food[i].active = false;
}

bool Simulation::Save(){
	unsigned int foodSize, critSize;

	foodSize = food.size(); critSize = critter.size();
	FILE *file = fopen("save/simulation.bin","wb");
	if(file == NULL){ fprintf(stderr,"Could not open save/simulation.bin for writing."); return false; }
	if(fwrite((void*)(&params),sizeof(SimulationParams),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&frameCount),sizeof(uint64_t),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&foodCount),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&critCount),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&foodSize),sizeof(unsigned int),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&critSize),sizeof(unsigned int),1,file)!=1){ fclose(file); return false; }
	fclose(file);

	for(unsigned int n=0; n<foodSize; n++){
		if(!food.at(n).Save(n))return false;
	}

	for(unsigned int n=0; n<critSize; n++){
		if(!critter.at(n).Save(n))return false;
	}

	return true;
}

bool Simulation::Load(){
	unsigned int foodSize, critSize;

	Clear();

	foodSize = food.size(); critSize = critter.size();
	FILE *file = fopen("save/simulation.bin","rb");
	if(file==NULL)return false;
	if(fread((void*)(&params),sizeof(SimulationParams),1,file)!=1){ fclose(file); Clear(); return false; }
	if(fread((void*)(&frameCount),sizeof(uint64_t),1,file)!=1){ fclose(file); Clear(); return false; }
	if(fread((void*)(&foodCount),sizeof(uint32_t),1,file)!=1){ fclose(file); Clear(); return false; }
	if(fread((void*)(&critCount),sizeof(uint32_t),1,file)!=1){ fclose(file); Clear(); return false; }
	if(fread((void*)(&foodSize),sizeof(unsigned int),1,file)!=1){ fclose(file); Clear(); return false; }
	if(fread((void*)(&critSize),sizeof(unsigned int),1,file)!=1){ fclose(file); Clear(); return false; }
	fclose(file);

	food.resize(foodSize); food.reserve(params.foodMaxInWorld + FOODRESERVE);
	for(unsigned int n=0; n<foodSize; n++){
		if(!food.at(n).Load(n)){ Clear(); return false; }
	}

	critter.resize(critSize); critter.reserve(params.critMaxInWorld + CRITRESERVE);
	for(unsigned int n=0; n<critSize; n++){
		if(!critter.at(n).Load(n)){ Clear(); return false; }
	}

	return true;
}

uint64_t Simulation::GetFrameCount(){
	return frameCount;
}

uint32_t Simulation::GetCritterCount(){
	return critCount;
}

uint32_t Simulation::GetFoodCount(){
	return foodCount;
}

float Simulation::GetEnergy(){
	float energy = 0.;
	for(uint32_t n=0; n<critter.size(); n++){
		if(!critter[n].active)continue;
		energy += critter[n].energy;
		energy += critter[n].waste;
	}
	for(uint32_t n=0; n<food.size(); n++){
		if(!food[n].active)continue;
		energy += food[n].energy;
	}
	return energy;
}

bool Simulation::Click(float x, float y){
	for(uint32_t n=0; n<critter.size(); n++){
		if(!critter[n].active)continue;
		if(pow(pow(x-critter.at(n).x,2)+pow(y-critter.at(n).y,2),2)<pow(critter.at(n).rad,2)){
			critSelected = true;
			critSelect = n;
			return true;
		}
	}
	critSelected = false;
	critSelect = 0;
	return false;
}

void Simulation::DrawBrainGUI(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2){
	Critter *crit = (critSelected&&(critSelect<critter.size()))?&critter[critSelect]:NULL;
	if(crit == NULL){
		sprintf(buffer,"No Critter selected");
	}else{
		sprintf(buffer,"Selected critter: %d\n",critSelect);
	}
	brainTextTop.Render(buffer,0x000000FF,13);
	brainTextTop.Blit(dst,posX1,posY1,posX2-posX1,20,true,true);
	if(crit == NULL)return;
	int pxsize = posX2-posX1;

	crit->net.Draw(dst,posX1,posY1+30,posX2,posY1+pxsize);
	sprintf(buffer,
		"gen_netSize = %d gen_radius = %+3.6f\n"
		"gen_growth = %+3.6f gen_speed = %+3.6f\n"
		"in_random = %+3.6f in_size = %+3.6f\n"
		"in_energy = %+3.6f in_waste = %+3.6f\n"
		"in_age = %+3.6f in_foodNearAng = %+3.6f\n"
		"in_foodNearDis = %+3.6f in_foodAng = %+3.6f\n"
		"in_critNearAng = %+3.6f in_critNearDis = %+3.6f\n"
		"in_critAng = %+3.6f\n"
		"in_sensorU = %+3.6f in_sensorUR = %+3.6f\n"
		"in_sensorR = %+3.6f in_sensorDR = %+3.6f\n"
		"in_sensorD = %+3.6f in_sensorDL = %+3.6f\n"
		"in_sensorL = %+3.6f in_sensorUL = %+3.6f\n"
		"out_move = %+3.6f out_spin = %+3.6f\n"
		"out_mood = %+3.6f out_expose = %+3.6f\n"
		"x = %+3.6f y = %+3.6f rot = %+3.6f\n"
		,crit->gen_netSize, crit->gen_radius
		,crit->gen_growth, crit->gen_speed
		,crit->in_random, crit->in_size
		,crit->in_energy, crit->in_waste
		,crit->in_age, crit->in_foodNearAng
		,crit->in_foodNearDis, crit->in_foodAng
		,crit->in_critNearAng, crit->in_critNearDis
		,crit->in_critAng
		,crit->in_sensorU, crit->in_sensorUR
		,crit->in_sensorR, crit->in_sensorDR
		,crit->in_sensorD, crit->in_sensorDL
		,crit->in_sensorL, crit->in_sensorUL
		,crit->out_move, crit->out_spin
		,crit->out_mood, crit->out_expose
		,crit->x, crit->y, crit->rot
		);
	brainText.Render(buffer,0x000000FF,13);
	brainText.Blit(dst,posX1,posY1+pxsize+10,posX2-posX1,posY2-posY1-pxsize-10,false,false);
}

void Simulation::DrawInfoGUI(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2){
	sprintf(buffer,
		"Casfire Artificial Life v1.1 - 2012\n"
		"http://casfire.com\n\n"
		"Critters: %u\n"
		"Food: %u\n"
		"Frame: %"PRIu64
		,critCount
		,foodCount
		,frameCount
	);

	if(critSelected){
		infoText.Render(buffer,0x000000FF,13);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,posY2-posY1,false,false);

		Critter *c = &critter[critSelect];
		float xsize = (posX2 - posX1);
		Uint32 fillColor = 0x00AA00FF, badColor = 0xFF0000FF, goodColor = 0x00FF00FF;

		c->Draw(dst,posX1 + xsize*0.8,posY1 + xsize*0.2 - 10,xsize*0.2);

		int px1 = posX1 + 100, px2 = posX2, pxh = (px1+px2)/2;
		xsize = (px2 - px1);

		posY1 += 170;

		infoText.Render("Size:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		boxColor(dst,px1,posY1,px1+(xsize*c->in_size),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Hunger:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		boxColor(dst,px1,posY1,px1+(xsize*(1.-c->in_energy)),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Waste:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		boxColor(dst,px1,posY1,px1+(xsize*c->in_waste),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Age:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		boxColor(dst,px1,posY1,px1+(xsize*c->in_age),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Membrane:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		boxColor(dst,px1,posY1,px1+(xsize*(0.5*(c->out_expose+1.))),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		float val;

		infoText.Render("Mood:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		val = c->out_mood;
		if(val<0.)boxColor(dst,px1+(xsize*0.5*(val+1.)),posY1,pxh,posY1+15,badColor);
		else boxColor(dst,pxh,posY1,pxh+xsize*(val*0.5),posY1+15,goodColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Spin:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		val = c->out_spin;
		if(val<0.)boxColor(dst,px1+(xsize*0.5*(val+1.)),posY1,pxh,posY1+15,fillColor);
		else boxColor(dst,pxh,posY1,pxh+xsize*(val*0.5),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;

		infoText.Render("Speed:",0x000000FF,11);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,15,false,true);
		val = c->out_move;
		if(val<0.)boxColor(dst,px1+(xsize*0.5*(val+1.)),posY1,pxh,posY1+15,fillColor);
		else boxColor(dst,pxh,posY1,pxh+xsize*(val*0.5),posY1+15,fillColor);
		rectangleColor(dst,px1,posY1,px2,posY1+15,0x000000FF);
		posY1 += 20;
	}else{
		infoText.Render(buffer,0x000000FF,13);
		infoText.Blit(dst,posX1,posY1,posX2-posX1,posY2-posY1,true,true);
	}
}
