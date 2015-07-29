#include "critter.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#define M_PIDIV4 0.7853981633974483096156608458198 // (M_PI/4.)
#define M_PIDIV2 1.5707963267948966192313216916397 // (M_PI/2.)
#define M_PI3DIV4 2.3561944901923449288469825374596 // (M_PI*(3./4.))

Critter::Critter(){
	active = false;
	gen_radius = 0.; gen_growth = 0.; gen_speed = 0.; gen_netSize = 0;
	out_move = 0.; out_spin = 0.; out_mood = 0.; out_expose = 0.;
	in_random = 0.; in_size = 0.; in_energy = 0.; in_waste = 0.; in_age = 0.;
	in_foodNearAng = 0.; in_foodNearDis = 0.; in_foodAng = 0.; in_critNearAng = 0.; in_critNearDis = 0.; in_critAng = 0.;
	in_sensorU = 0.; in_sensorUR = 0.; in_sensorR = 0.; in_sensorDR = 0.; in_sensorD = 0.; in_sensorDL = 0.; in_sensorL = 0.; in_sensorUL = 0.;
	x = 0.; y = 0.; rot = 0.;
	rad = 0.; mrad = 0.; mradPow = 0.;
	energy = 0.; waste = 0.;
	maxEnergy = 0.; maxGenEnergy = 0.; processedEnergy = 0.;
	tmp_foodX = 0.; tmp_foodY = 0.; tmp_foodClosestDistance = 0.; tmp_foodClosest = NULL;
	tmp_critX = 0.; tmp_critY = 0.; tmp_critClosestDistance = 0.; tmp_critClosest = NULL;
	tmp_exp = 0.;
	tmp_sensorUx = 0.; tmp_sensorUy = 0.; tmp_sensorURx = 0.; tmp_sensorURy = 0.;
	tmp_sensorRx = 0.; tmp_sensorRy = 0.; tmp_sensorDRx = 0.; tmp_sensorDRy = 0.;
	tmp_sensorDx = 0.; tmp_sensorDy = 0.; tmp_sensorDLx = 0.; tmp_sensorDLy = 0.;
	tmp_sensorLx = 0.; tmp_sensorLy = 0.; tmp_sensorULx = 0.; tmp_sensorULy = 0.;
}

Critter::~Critter(){
}

void Critter::Spawn(float posX, float posY, SimulationParams *params){
	gen_netSize = params->critMinGenNetSize + (params->rnd.Next()%(params->critMaxGenNetSize-params->critMinGenNetSize));
	net.SetSize(gen_netSize + CRITTER_NODES);
	net.SetNodeAll(0.);
	net.SetWeightsRandom(&params->rnd);
	net.SetParamsRandom(&params->rnd);
	gen_radius = params->rnd.xrand(params->critMinGenRadius,params->critMaxGenRadius);
	gen_growth = params->rnd.xrand(params->critMinGenGrowth,params->critMaxGenGrowth);
	gen_speed = params->rnd.xrand(params->critMinGenSpeed,params->critMaxGenSpeed);

	out_move = 0.; out_spin = 0.; out_mood = 0.; out_expose = 0.;
	in_random = 0.; in_size = 0.; in_energy = 0.; in_waste = 0.; in_age = 0.;
	in_foodNearAng = 0.; in_foodNearDis = 0.; in_foodAng = 0.; in_critNearAng = 0.; in_critNearDis = 0.; in_critAng = 0.;
	in_sensorU = 0.; in_sensorUR = 0.; in_sensorR = 0.; in_sensorDR = 0.; in_sensorD = 0.; in_sensorDL = 0.; in_sensorL = 0.; in_sensorUL = 0.;

	x = posX; y = posY; rot = params->rnd.xrand(-M_PI,M_PI);
	rad = gen_radius; mrad = rad; mradPow = pow(rad,2);
	energy = M_PI*mradPow; waste = 0.;
	maxEnergy = energy; maxGenEnergy = energy; processedEnergy = 0.;

	tmp_foodX = 0.; tmp_foodY = 0.; tmp_foodClosestDistance = params->simRadius*2; tmp_foodClosest = NULL;
	tmp_critX = 0.; tmp_critY = 0.; tmp_critClosestDistance = params->simRadius*2; tmp_critClosest = NULL;
	tmp_exp = 0.;
	tmp_sensorUx = 0.; tmp_sensorUy = 0.; tmp_sensorURx = 0.; tmp_sensorURy = 0.;
	tmp_sensorRx = 0.; tmp_sensorRy = 0.; tmp_sensorDRx = 0.; tmp_sensorDRy = 0.;
	tmp_sensorDx = 0.; tmp_sensorDy = 0.; tmp_sensorDLx = 0.; tmp_sensorDLy = 0.;
	tmp_sensorLx = 0.; tmp_sensorLy = 0.; tmp_sensorULx = 0.; tmp_sensorULy = 0.;
}

void Critter::Spawn(float posX, float posY, Critter *parentA, Critter *parentB, SimulationParams *params){
	gen_netSize = (uint32_t)(((((float)parentA->gen_netSize) + ((float)parentB->gen_netSize))/2.) + params->rnd.xrand(0.,1.));
	if(gen_netSize<params->critMinGenNetSize)gen_netSize = params->critMinGenNetSize;
	if(gen_netSize>params->critMaxGenNetSize)gen_netSize = params->critMaxGenNetSize;
	float tmp = params->rnd.xrand(-1.,1.);
	if(fabs(tmp)<params->critMutateSize)gen_netSize += (tmp<0.)?1:-1;
	net.Crossover(&parentA->net,&parentB->net);
	net.SetNodeAll(0.);
	net.Mutate(&params->rnd,params->critMutateSize,params->critMutateDiv);
	uint32_t i = net.GetSize();
	if(gen_netSize!=i){
		net.SetSize(gen_netSize + CRITTER_NODES);
		if(gen_netSize>i){
			net.SetNode(i,0.);
			net.SetParams(i,params->rnd.xrand(-1.,1.),params->rnd.xrand(-1.,1.),params->rnd.xrand(-1.,1.));
			for(uint32_t n=0; n<net.GetSize(); n++){
				net.SetWeight(n,i,0.); //net.SetWeight(n,i,params->rnd.xrand(-1.,1.));
				net.SetWeight(i,n,0.); //net.SetWeight(i,n,params->rnd.xrand(-1.,1.));
			}
		}
	}
	gen_radius = ((parentA->gen_radius+parentB->gen_radius)/2.) + params->rnd.xrand(-params->critMutateSize,params->critMutateSize);
	if(gen_radius<params->critMinGenRadius)gen_radius = params->critMinGenRadius;
	if(gen_radius>params->critMaxGenRadius)gen_radius = params->critMaxGenRadius;
	gen_growth = ((parentA->gen_growth+parentB->gen_growth)/2.);
	if(gen_growth<params->critMinGenGrowth)gen_growth = params->critMinGenGrowth;
	if(gen_growth>params->critMaxGenGrowth)gen_growth = params->critMaxGenGrowth;
	gen_speed = ((parentA->gen_speed+parentB->gen_speed)/2.);
	if(gen_speed<params->critMinGenSpeed)gen_speed = params->critMinGenSpeed;
	if(gen_speed>params->critMaxGenSpeed)gen_speed = params->critMaxGenSpeed;

	out_move = 0.; out_spin = 0.; out_mood = 0.; out_expose = 0.;
	in_random = 0.; in_size = 0.; in_energy = 0.; in_waste = 0.; in_age = 0.;
	in_foodNearAng = 0.; in_foodNearDis = 0.; in_foodAng = 0.; in_critNearAng = 0.; in_critNearDis = 0.; in_critAng = 0.;
	in_sensorU = 0.; in_sensorUR = 0.; in_sensorR = 0.; in_sensorDR = 0.; in_sensorD = 0.; in_sensorDL = 0.; in_sensorL = 0.; in_sensorUL = 0.;

	maxGenEnergy = M_PI*pow(gen_radius,2);
	float takeFromParents = maxGenEnergy/(parentA->energy + parentB->energy);
	if(params->critEnergyBirthFromParents<takeFromParents)takeFromParents = params->critEnergyBirthFromParents;
	float takeEnergyA = parentA->energy*takeFromParents;
	float takeEnergyB = parentB->energy*takeFromParents;
	parentA->energy -= takeEnergyA;
	parentB->energy -= takeEnergyB;

	x = posX; y = posY; rot = params->rnd.xrand(-M_PI,M_PI);
	energy = takeEnergyA + takeEnergyB; waste = 0.;
	mradPow = energy/M_PI; rad = sqrt(mradPow); mrad = rad;
	maxEnergy = energy; processedEnergy = 0.;

	tmp_foodX = 0.; tmp_foodY = 0.; tmp_foodClosestDistance = params->simRadius*2; tmp_foodClosest = NULL;
	tmp_critX = 0.; tmp_critY = 0.; tmp_critClosestDistance = params->simRadius*2; tmp_critClosest = NULL;
	tmp_exp = 0.;
	tmp_sensorUx = 0.; tmp_sensorUy = 0.; tmp_sensorURx = 0.; tmp_sensorURy = 0.;
	tmp_sensorRx = 0.; tmp_sensorRy = 0.; tmp_sensorDRx = 0.; tmp_sensorDRy = 0.;
	tmp_sensorDx = 0.; tmp_sensorDy = 0.; tmp_sensorDLx = 0.; tmp_sensorDLy = 0.;
	tmp_sensorLx = 0.; tmp_sensorLy = 0.; tmp_sensorULx = 0.; tmp_sensorULy = 0.;
}

bool Critter::Frame(Simulation *simulation){
	
	SimulationParams *params = &simulation->params;

	//inputs
	in_random = params->rnd.xrand(-1.,1.);
	in_size = rad/gen_radius;
	in_energy = energy/maxEnergy;
	in_waste = waste/maxEnergy;
	in_age = processedEnergy/(params->critMaxProccessedEnergy*maxGenEnergy);
	in_foodAng = atan2(tmp_foodY,tmp_foodX)-rot; while(in_foodAng<-M_PI)in_foodAng+=M_PI*2.; while(in_foodAng>M_PI)in_foodAng-=M_PI*2.; in_foodAng/=M_PI;
	in_critAng = atan2(tmp_critY,tmp_critX)-rot; while(in_critAng<-M_PI)in_critAng+=M_PI*2.; while(in_critAng>M_PI)in_critAng-=M_PI*2.; in_critAng/=M_PI;
	in_foodNearAng = 0.; in_foodNearDis = 1.;
	in_critNearAng = 0.; in_critNearDis = 1.;
	if(tmp_foodClosest!=NULL){
		in_foodNearAng = atan2(tmp_foodClosest->y-y,tmp_foodClosest->x-x)-rot;
		while(in_foodNearAng<-M_PI)in_foodNearAng+=M_PI*2.; while(in_foodNearAng>M_PI)in_foodNearAng-=M_PI*2.; in_foodNearAng/=M_PI;
		tmp_foodClosestDistance = (tmp_foodClosestDistance/rad)-1.;
		in_foodNearDis = tmp_foodClosestDistance/(tmp_foodClosestDistance+1.);
		if(in_foodNearDis < 0.)in_foodNearDis = 0.;
	}
	if(tmp_critClosest!=NULL){
		in_critNearAng = atan2(tmp_critClosest->y-y,tmp_critClosest->x-x)-rot;
		while(in_critNearAng<-M_PI)in_critNearAng+=M_PI*2.; while(in_critNearAng>M_PI)in_critNearAng-=M_PI*2.; in_critNearAng/=M_PI;
		tmp_critClosestDistance = (tmp_critClosestDistance/rad)-1.;
		in_critNearDis = tmp_critClosestDistance/(tmp_critClosestDistance+1.);
		if(in_critNearDis < 0.)in_critNearDis = 0.;
	}

	//set inputs
	net.SetNode(0 ,in_random);
	net.SetNode(1 ,in_size);
	net.SetNode(2 ,in_energy);
	net.SetNode(3 ,in_waste);
	net.SetNode(4 ,in_age);
	net.SetNode(5 ,in_foodNearAng);
	net.SetNode(6 ,in_foodNearDis);
	net.SetNode(7 ,in_foodAng);
	net.SetNode(8 ,in_critNearAng);
	net.SetNode(9 ,in_critNearDis);
	net.SetNode(10,in_critAng);
	net.SetNode(11,in_sensorU);
	net.SetNode(12,in_sensorUL);
	net.SetNode(13,in_sensorL);
	net.SetNode(14,in_sensorDL);
	net.SetNode(15,in_sensorD);
	net.SetNode(16,in_sensorDR);
	net.SetNode(17,in_sensorR);
	net.SetNode(18,in_sensorUR);
	net.Step();

	//get outputs
	float out_moveChange   = out_move;   out_move =   net.GetNode(19); out_moveChange   = fabs(out_move - out_moveChange) * 0.5;
	float out_spinChange   = out_spin;   out_spin =   net.GetNode(20); out_spinChange   = fabs(out_spin - out_spinChange) * 0.5;
	float out_moodChange   = out_mood;   out_mood =   net.GetNode(21); out_moodChange   = fabs(out_mood - out_moodChange) * 0.5;
	float out_exposeChange = out_expose; out_expose = net.GetNode(22); out_exposeChange = out_expose - out_exposeChange * 0.5;

	//spin
	rot += out_spin * params->critMultiplierSpin * M_PI;
	while(rot<-M_PI)rot+=M_PI*2; while(rot>M_PI)rot-=M_PI*2;

	tmp_exp = ((out_expose+1.)/2.); //out_expose from [-1,1] to [0,1]

	//movement
	bool isForward = (out_move>0.);
	float move = tmp_exp * out_move * rad * gen_speed * ((isForward)?params->critMultiplierMoveForward:params->critMultiplierMoveBackward);
	x += cos(rot) * move;
	y += sin(rot) * move;
	if((x*x+y*y)>pow(params->simRadius-rad,2)){
		float a = atan2(y,x);
		x = cos(a)*(params->simRadius-rad);
		y = sin(a)*(params->simRadius-rad);
	}

	//growth
	rad += (gen_radius - rad) * gen_growth;
	mrad = rad + rad*tmp_exp;
	mradPow = pow(mrad,2);
	maxEnergy = M_PI*pow(rad,2);

	//energy consumption
	float consume = 0.;
	consume += rad * maxEnergy * params->critEnergyToLive;
	consume += rad * fabs(out_move) * pow(gen_speed,2) * maxEnergy * ((out_move>0.)?(params->critEnergyToMoveForw):(params->critEnergyToMoveBack));
	consume += rad * fabs(out_spin) * maxEnergy * params->critEnergyToSpin;
	consume += rad * fabs(out_mood) * maxEnergy * ((out_mood>0.)?params->critEnergyToMoodGood:params->critEnergyToMoodBad);
	consume += rad * tmp_exp        * maxEnergy * params->critEnergyToExpose;
	consume += out_moveChange       * maxEnergy * params->critEnergyToChangeMove;
	consume += out_spinChange       * maxEnergy * params->critEnergyToChangeSpin;
	consume += out_moodChange       * maxEnergy * params->critEnergyToChangeMood;
	consume += fabs(out_exposeChange) * maxEnergy * ((out_exposeChange<0.)?params->critEnergyToDecreaseExpose:params->critEnergyToIncreaseExpose);
	consume += gen_netSize * maxEnergy * params->critEnergyToBrain;
	bool ret = false;
	if(consume>energy){
		consume = energy;
		ret = true; //death from hunger
	}
	waste += consume;
	energy -= consume;
	while(waste>=maxEnergy){
		simulation->AddFood(x,y,0.,maxEnergy,rot+M_PI);
		waste -= maxEnergy;
	}
	processedEnergy += consume;
	if(processedEnergy >= params->critMaxProccessedEnergy * maxGenEnergy){
		processedEnergy = params->critMaxProccessedEnergy * maxGenEnergy;
		return true; //death from age
	}
	if(ret)return true;

	tmp_foodX = 0.; tmp_foodY = 0.; tmp_foodClosestDistance = params->simRadius*2; tmp_foodClosest = NULL;
	tmp_critX = 0.; tmp_critY = 0.; tmp_critClosestDistance = params->simRadius*2; tmp_critClosest = NULL;
	in_sensorU = 0.; in_sensorUR = 0.; in_sensorR = 0.; in_sensorDR = 0.; in_sensorD = 0.; in_sensorDL = 0.; in_sensorL = 0.; in_sensorUL = 0.;

	//sensor positions
	tmp_sensorUx  = x+cos(rot          )*mrad; tmp_sensorUy  = y+sin(rot          )*mrad;
	tmp_sensorURx = x+cos(rot-M_PIDIV4 )*mrad; tmp_sensorURy = y+sin(rot-M_PIDIV4 )*mrad;
	tmp_sensorRx  = x+cos(rot-M_PIDIV2 )*mrad; tmp_sensorRy  = y+sin(rot-M_PIDIV2 )*mrad;
	tmp_sensorDRx = x+cos(rot-M_PI3DIV4)*mrad; tmp_sensorDRy = y+sin(rot-M_PI3DIV4)*mrad;
	tmp_sensorDx  = x+cos(rot+M_PI     )*mrad; tmp_sensorDy  = y+sin(rot+M_PI     )*mrad;
	tmp_sensorDLx = x+cos(rot+M_PI3DIV4)*mrad; tmp_sensorDLy = y+sin(rot+M_PI3DIV4)*mrad;
	tmp_sensorLx  = x+cos(rot+M_PIDIV2 )*mrad; tmp_sensorLy  = y+sin(rot+M_PIDIV2 )*mrad;
	tmp_sensorULx = x+cos(rot+M_PIDIV4 )*mrad; tmp_sensorULy = y+sin(rot+M_PIDIV4 )*mrad;

	return false;
}

void Critter::Interact(Critter *crit, Simulation *simulation){
	if(crit->energy <= 0.)return;
	if(energy <= 0.)return;

	SimulationParams *params = &simulation->params;
	Critter *A = this;
	Critter *B = crit;

	float dxA = crit->x - x, dxB = -dxA;
	float dyA = crit->y - y, dyB = -dyA;
	float dxp = pow(dxA,2), dyp = pow(dyA,2);
	float r = dxp + dyp, rp = pow(r,2); if(rp < A->rad + B->rad)rp = A->rad + B->rad;
	A->tmp_critX += B->energy*dxA/rp;
	A->tmp_critY += B->energy*dyA/rp;
	B->tmp_critX += A->energy*dxB/rp;
	B->tmp_critY += A->energy*dyB/rp;
	float dis = sqrt(r); //distance between centers
	if(dis-B->rad<A->tmp_critClosestDistance){ A->tmp_critClosest = B; A->tmp_critClosestDistance = dis-B->rad; }
	if(dis-A->rad<B->tmp_critClosestDistance){ B->tmp_critClosest = A; B->tmp_critClosestDistance = dis-A->rad; }
	float disMem = dis-A->mrad-B->mrad; //distance between membranes

	if(disMem<=0.){

		if(A->in_sensorU ==0.)if(pow(B->x-A->tmp_sensorUx ,2)+pow(B->y-A->tmp_sensorUy ,2)<B->mradPow)A->in_sensorU	= B->out_mood;
		if(A->in_sensorUR==0.)if(pow(B->x-A->tmp_sensorURx,2)+pow(B->y-A->tmp_sensorURy,2)<B->mradPow)A->in_sensorUR = B->out_mood;
		if(A->in_sensorR ==0.)if(pow(B->x-A->tmp_sensorRx ,2)+pow(B->y-A->tmp_sensorRy ,2)<B->mradPow)A->in_sensorR	= B->out_mood;
		if(A->in_sensorDR==0.)if(pow(B->x-A->tmp_sensorDRx,2)+pow(B->y-A->tmp_sensorDRy,2)<B->mradPow)A->in_sensorDR = B->out_mood;
		if(A->in_sensorD ==0.)if(pow(B->x-A->tmp_sensorDx ,2)+pow(B->y-A->tmp_sensorDy ,2)<B->mradPow)A->in_sensorD	= B->out_mood;
		if(A->in_sensorDL==0.)if(pow(B->x-A->tmp_sensorDLx,2)+pow(B->y-A->tmp_sensorDLy,2)<B->mradPow)A->in_sensorDL = B->out_mood;
		if(A->in_sensorL ==0.)if(pow(B->x-A->tmp_sensorLx ,2)+pow(B->y-A->tmp_sensorLy ,2)<B->mradPow)A->in_sensorL	= B->out_mood;
		if(A->in_sensorUL==0.)if(pow(B->x-A->tmp_sensorULx,2)+pow(B->y-A->tmp_sensorULy,2)<B->mradPow)A->in_sensorUL = B->out_mood;

		if(B->in_sensorU ==0.)if(pow(A->x-B->tmp_sensorUx ,2)+pow(A->y-B->tmp_sensorUy ,2)<A->mradPow)B->in_sensorU	= A->out_mood;
		if(B->in_sensorUR==0.)if(pow(A->x-B->tmp_sensorURx,2)+pow(A->y-B->tmp_sensorURy,2)<A->mradPow)B->in_sensorUR = A->out_mood;
		if(B->in_sensorR ==0.)if(pow(A->x-B->tmp_sensorRx ,2)+pow(A->y-B->tmp_sensorRy ,2)<A->mradPow)B->in_sensorR	= A->out_mood;
		if(B->in_sensorDR==0.)if(pow(A->x-B->tmp_sensorDRx,2)+pow(A->y-B->tmp_sensorDRy,2)<A->mradPow)B->in_sensorDR = A->out_mood;
		if(B->in_sensorD ==0.)if(pow(A->x-B->tmp_sensorDx ,2)+pow(A->y-B->tmp_sensorDy ,2)<A->mradPow)B->in_sensorD	= A->out_mood;
		if(B->in_sensorDL==0.)if(pow(A->x-B->tmp_sensorDLx,2)+pow(A->y-B->tmp_sensorDLy,2)<A->mradPow)B->in_sensorDL = A->out_mood;
		if(B->in_sensorL ==0.)if(pow(A->x-B->tmp_sensorLx ,2)+pow(A->y-B->tmp_sensorLy ,2)<A->mradPow)B->in_sensorL	= A->out_mood;
		if(B->in_sensorUL==0.)if(pow(A->x-B->tmp_sensorULx,2)+pow(A->y-B->tmp_sensorULy,2)<A->mradPow)B->in_sensorUL = A->out_mood;

		float Astrength = A->energy/(A->energy + B->energy);
		float Bstrength = 1. - Astrength;
		bool Amood = fabs(A->out_mood)>params->critMoodThershold;
		bool Bmood = fabs(B->out_mood)>params->critMoodThershold;

		if(Amood&&Bmood&&(A->out_mood>0.)&&(B->out_mood>0.)){
			bool Abirth = (A->energy > params->critEnergyMinToGiveBirth*A->maxEnergy)&&(processedEnergy>maxEnergy*params->critEnergyMinProcessedToGiveBirth);
			bool Bbirth = (A->energy > params->critEnergyMinToGiveBirth*A->maxEnergy)&&(processedEnergy>maxEnergy*params->critEnergyMinProcessedToGiveBirth);
			if(Abirth&&Bbirth)simulation->AddCritter((A->x+B->x)/2.,(A->y+B->y)/2.,this,crit);
		}

		if(Amood&&(A->out_mood<0.)){
			float take = (-A->out_mood) * Astrength * B->tmp_exp * B->maxEnergy * params->critAttackEnergyTake;
			if(take>B->energy)take = B->energy;
			if(take>A->maxEnergy-A->energy)take=A->maxEnergy-A->energy;
			A->energy += take;
			B->energy -= take;
		}

		if(Bmood&&(B->out_mood<0.)){
			float take = (-B->out_mood) * Bstrength * A->tmp_exp * A->maxEnergy * params->critAttackEnergyTake;
			if(take>A->energy)take = A->energy;
			if(take>B->maxEnergy-B->energy)take=B->maxEnergy-B->energy;
			B->energy += take;
			A->energy -= take;
		}

		float disIn = dis - A->rad - B->rad; //distance between bodies
		if(disIn<=0.){
			float sizeA = M_PI*pow(A->rad,2);
			float sizeB = M_PI*pow(B->rad,2);
			float r1 = (sizeA)/(sizeA+sizeB);
			float r2 = (sizeB)/(sizeA+sizeB);
			float mov = (A->rad + B->rad - dis);
			float ang = atan2(A->y - B->y, A->x - B->x);
			A->x += cos(ang)*mov*r1;
			A->y += sin(ang)*mov*r1;
			ang += M_PI;
			B->x += cos(ang)*mov*r2;
			B->y += sin(ang)*mov*r2;
		}

	}

}

bool Critter::Interact(Food *food, SimulationParams *params){
	params = params; //avoid warnings
	float dx = food->x - x, dxp = pow(dx,2);
	float dy = food->y - y, dyp = pow(dy,2);
	float r = dxp + dyp;
	if(food->isFood){
		float rp = pow(r,2);
		tmp_foodX += food->energy*dx/rp;
		tmp_foodY += food->energy*dy/rp;
	}
	float dis = sqrt(r);
	if(dis-food->rad<tmp_foodClosestDistance){ tmp_foodClosest = food; tmp_foodClosestDistance = dis-food->rad; }
	if(dis<food->rad+rad){
		if((energy<maxEnergy)&&food->isFood){
			float takeEnergy = food->energy;
			if(dis>rad)takeEnergy -= M_PI*pow(rad-dis,2);
			float max = maxEnergy - energy;
			if(takeEnergy>max)takeEnergy = max;
			energy += takeEnergy;
			food->energy -= takeEnergy;
			if(food->energy<=0.){
				food->rad = 0.;
				return true;
			}else{
				food->rad = sqrt(food->energy/M_PI);
			}
		}
		if(dis<food->rad+rad){
			//move outside
			float angA = atan2(y-food->y,x-food->x);
			float angB = angA + M_PI;
			float mass = food->energy + energy;
			float Ar = food->energy/mass;
			float Br = energy/mass;
			float m = food->rad+rad-dis;
			x+=cos(angA)*m*Ar;
			y+=sin(angA)*m*Ar;
			food->x+=cos(angB)*m*Br;
			food->y+=sin(angB)*m*Br;
			float speed = sqrt(pow(food->sx,2)+pow(food->sy,2));
			food->sx = cos(angB)*speed;
			food->sy = sin(angB)*speed;
		}
	}
	return false;
}

bool Critter::Save(unsigned int n){
	char filename[40];
	sprintf(filename,"save/%u.net",n);
	if(!net.SaveToFile(filename))return false;
	sprintf(filename,"save/%u.crit",n);
	FILE *file = fopen(filename,"wb");
	if(file==NULL)return false;
	if(fwrite((void*)(&gen_netSize),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&gen_radius),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&gen_growth),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&gen_speed),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&out_move),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&out_spin),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&out_mood),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&out_expose),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_random),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_size),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_waste),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_age),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_foodNearAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_foodNearDis),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_foodAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_critNearAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_critNearDis),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_critAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorU),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorUR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorDR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorD),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorDL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&in_sensorUL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&x),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&y),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&rot),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&rad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&mrad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&mradPow),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&waste),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&maxEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&maxGenEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&processedEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorUx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorUy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorURx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorURy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorRx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorRy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDRx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDRy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDLx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorDLy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorLx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorLy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorULx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&tmp_sensorULy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fwrite((void*)(&active),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	fclose(file);
	return true;
}

bool Critter::Load(unsigned int n){
	char filename[40];

	sprintf(filename,"save/%u.net",n);
	if(!net.LoadFromFile(filename))return false;

	sprintf(filename,"save/%u.crit",n);
	FILE *file = fopen(filename,"rb");
	if(file==NULL)return false;
	if(fread((void*)(&gen_netSize),sizeof(uint32_t),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&gen_radius),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&gen_growth),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&gen_speed),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&out_move),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&out_spin),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&out_mood),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&out_expose),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_random),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_size),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_waste),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_age),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_foodNearAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_foodNearDis),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_foodAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_critNearAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_critNearDis),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_critAng),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorU),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorUR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorDR),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorD),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorDL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&in_sensorUL),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&x),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&y),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&rot),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&rad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&mrad),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&mradPow),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&energy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&waste),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&maxEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&maxGenEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&processedEnergy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorUx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorUy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorURx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorURy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorRx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorRy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDRx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDRy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDLx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorDLy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorLx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorLy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorULx),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&tmp_sensorULy),sizeof(float),1,file)!=1){ fclose(file); return false; }
	if(fread((void*)(&active),sizeof(bool),1,file)!=1){ fclose(file); return false; }
	fclose(file);

	return true;
}

void Critter::Draw(SDL_Surface *dst, MovableView *view, bool selected){

	Uint32 mGood = 0x00FF0044; //good mood color
	Uint32 mBad	= 0xFF000044; //bad mood color

	SDL_Color from, to; Uint8 r,g,b,a; float m;

	//membrane
	from = Uint32ToColor(mBad);
	to = Uint32ToColor(mGood);
	m = (out_mood+1.)/2.;
	r = from.r + (int)((((float)to.r)-((float)from.r))*m );
	g = from.g + (int)((((float)to.g)-((float)from.g))*m );
	b = from.b + (int)((((float)to.b)-((float)from.b))*m );
	a = from.unused + (int)((((float)to.unused)-((float)from.unused))*m );
	view->filledCircle(dst,x,y,mrad,RGBA(r,g,b,a));

	//body
	view->filledCircle(dst,x,y,rad,0x3382C7FF);

	//energy
	view->filledPie(dst,x,y,rad,rot-(energy/maxEnergy)*M_PI,rot+(energy/maxEnergy)*M_PI-0.001,0xCCF888FF);

	//waste
	view->filledCircle(dst,x,y,rad*(waste/maxEnergy),0x814A1AFF);
	view->circle(dst,x,y,rad*(waste/maxEnergy),0x000000FF);

	//direction line
	view->line(dst,x,y,x+(cos(rot)*rad*2),y+(sin(rot)*rad*2),0x000000FF);

	if(selected)view->filledCircle(dst,x,y,rad,0x00FF00CC);

	//body outline
	view->circle(dst,x,y,rad,0x000000FF);

}

void Critter::Draw(SDL_Surface *dst, float xPos, float yPos, float dRad){

	Uint32 mGood = 0x00FF0044; //good mood color
	Uint32 mBad	= 0xFF000044; //bad mood color

	SDL_Color from, to; Uint8 r,g,b,a; float m;

	//membrane
	from = Uint32ToColor(mBad);
	to = Uint32ToColor(mGood);
	m = (out_mood+1.)/2.;
	r = from.r + (int)((((float)to.r)-((float)from.r))*m );
	g = from.g + (int)((((float)to.g)-((float)from.g))*m );
	b = from.b + (int)((((float)to.b)-((float)from.b))*m );
	a = from.unused + (int)((((float)to.unused)-((float)from.unused))*m );
	float dRadP = dRad*0.5;
	filledCircleColor(dst,xPos,yPos,(mrad/rad)*dRadP,RGBA(r,g,b,a));

	//body
	filledCircleColor(dst,xPos,yPos,dRadP,0x3382C7FF);

	//energy
	filledPieColor(dst,xPos,yPos,dRadP,(energy/maxEnergy)*-180-90,(energy/maxEnergy)*180-90,0xCCF888FF);

	//waste
	filledCircleColor(dst,xPos,yPos,dRadP*(waste/maxEnergy),0x814A1AFF);
	circleColor(dst,xPos,yPos,dRadP*(waste/maxEnergy),0x000000FF);

	//direction line
	lineColor(dst,xPos,yPos,xPos,yPos-dRad,0x000000FF);

	//body outline
	circleColor(dst,xPos,yPos,dRadP,0x000000FF);

}
