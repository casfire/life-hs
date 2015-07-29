#include "params.hpp"

Params::Params(){
	scrollPOS = 0; scr_x = 0; scr_y = 0; isRegistered = false;
}

Params::~Params(){
}

void Params::Init(SimulationParams *params){
	char buff[1024];

	scrollUP.SetPos(-440,-135,-230,-115); scrollUP.SetText("Up"); scrollUP.SetDisabled(true);
	scrollDOWN.SetPos(-220,-135,-10,-115); scrollDOWN.SetText("Down");

	initialRandomAlg.ClearItems(); initialRandomAlg.AddItem("RNDSTDLIB"); initialRandomAlg.AddItem("RNDLCG");
	initialRandomAlg.AddItem("RNDMWC"); initialRandomAlg.AddItem("RNDWELL"); initialRandomAlg.AddItem("RNDMT");
	initialRandomAlg.SetDropDownHeight(80);
	switch(params->initialRandomAlg){ case RNDSTDLIB: initialRandomAlg.SetSelected(0); break; case RNDLCG: initialRandomAlg.SetSelected(1); break;
	case RNDMWC: initialRandomAlg.SetSelected(2); break; case RNDWELL: initialRandomAlg.SetSelected(3); break; case RNDMT: initialRandomAlg.SetSelected(4); break; }
	lab_initialRandomAlg.GetText()->Render("Random generator",0x000000FF,14);
	lab_initialRandomAlg.SetCenterX(false); lab_initialRandomAlg.SetCenterY(true);

	sprintf(buff,"%u",params->initialRandomSeed);
	initialRandomSeed.SetMaxLines(1); initialRandomSeed.SetText(buff);
	lab_initialRandomSeed.GetText()->Render("Initial RND number",0x000000FF,14);
	lab_initialRandomSeed.SetCenterX(false); lab_initialRandomSeed.SetCenterY(true);

	sprintf(buff,"%u",params->initialFoodCount);
	initialFoodCount.SetMaxLines(1); initialFoodCount.SetText(buff);
	lab_initialFoodCount.GetText()->Render("Initial food count",0x000000FF,14);
	lab_initialFoodCount.SetCenterX(false); lab_initialFoodCount.SetCenterY(true);

	sprintf(buff,"%u",params->initialCritCount);
	initialCritCount.SetMaxLines(1); initialCritCount.SetText(buff);
	lab_initialCritCount.GetText()->Render("Initial critter count",0x000000FF,14);
	lab_initialCritCount.SetCenterX(false); lab_initialCritCount.SetCenterY(true);

	sprintf(buff,"%.324g",params->initialFoodDistribution);
	initialFoodDistribution.SetMaxLines(1); initialFoodDistribution.SetText(buff);
	lab_initialFoodDistribution.GetText()->Render("Initial food distribution",0x000000FF,14);
	lab_initialFoodDistribution.SetCenterX(false); lab_initialFoodDistribution.SetCenterY(true);

	sprintf(buff,"%.324g",params->simRadius);
	simRadius.SetMaxLines(1); simRadius.SetText(buff);
	lab_simRadius.GetText()->Render("Simulation radius",0x000000FF,14);
	lab_simRadius.SetCenterX(false); lab_simRadius.SetCenterY(true);

	sprintf(buff,"%.324g",params->foodFriction);
	foodFriction.SetMaxLines(1); foodFriction.SetText(buff);
	lab_foodFriction.GetText()->Render("Food friction",0x000000FF,14);
	lab_foodFriction.SetCenterX(false); lab_foodFriction.SetCenterY(true);

	sprintf(buff,"%.324g",params->foodElastic);
	foodElastic.SetMaxLines(1); foodElastic.SetText(buff);
	lab_foodElastic.GetText()->Render("Food elasticity",0x000000FF,14);
	lab_foodElastic.SetCenterX(false); lab_foodElastic.SetCenterY(true);

	sprintf(buff,"%.324g",params->foodSpawnMomentum);
	foodSpawnMomentum.SetMaxLines(1); foodSpawnMomentum.SetText(buff);
	lab_foodSpawnMomentum.GetText()->Render("Food spawn momentum",0x000000FF,14);
	lab_foodSpawnMomentum.SetCenterX(false); lab_foodSpawnMomentum.SetCenterY(true);

	sprintf(buff,"%u",params->foodConvertAge);
	foodConvertAge.SetMaxLines(1); foodConvertAge.SetText(buff);
	lab_foodConvertAge.GetText()->Render("Food convert age",0x000000FF,14);
	lab_foodConvertAge.SetCenterX(false); lab_foodConvertAge.SetCenterY(true);

	sprintf(buff,"%u",params->foodMaxInWorld);
	foodMaxInWorld.SetMaxLines(1); foodMaxInWorld.SetText(buff);
	lab_foodMaxInWorld.GetText()->Render("Max food in world",0x000000FF,14);
	lab_foodMaxInWorld.SetCenterX(false); lab_foodMaxInWorld.SetCenterY(true);

	sprintf(buff,"%u",params->critMinInWorld);
	critMinInWorld.SetMaxLines(1); critMinInWorld.SetText(buff);
	lab_critMinInWorld.GetText()->Render("Min critters in world",0x000000FF,14);
	lab_critMinInWorld.SetCenterX(false); lab_critMinInWorld.SetCenterY(true);

	sprintf(buff,"%u",params->critMaxInWorld);
	critMaxInWorld.SetMaxLines(1); critMaxInWorld.SetText(buff);
	lab_critMaxInWorld.GetText()->Render("Max critters in world",0x000000FF,14);
	lab_critMaxInWorld.SetCenterX(false); lab_critMaxInWorld.SetCenterY(true);

	sprintf(buff,"%u",params->critMinGenNetSize);
	critMinGenNetSize.SetMaxLines(1); critMinGenNetSize.SetText(buff);
	lab_critMinGenNetSize.GetText()->Render("Min critter net size",0x000000FF,14);
	lab_critMinGenNetSize.SetCenterX(false); lab_critMinGenNetSize.SetCenterY(true);

	sprintf(buff,"%u",params->critMaxGenNetSize);
	critMaxGenNetSize.SetMaxLines(1); critMaxGenNetSize.SetText(buff);
	lab_critMaxGenNetSize.GetText()->Render("Max critter net size",0x000000FF,14);
	lab_critMaxGenNetSize.SetCenterX(false); lab_critMaxGenNetSize.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMinGenRadius);
	critMinGenRadius.SetMaxLines(1); critMinGenRadius.SetText(buff);
	lab_critMinGenRadius.GetText()->Render("Min critter radius",0x000000FF,14);
	lab_critMinGenRadius.SetCenterX(false); lab_critMinGenRadius.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMaxGenRadius);
	critMaxGenRadius.SetMaxLines(1); critMaxGenRadius.SetText(buff);
	lab_critMaxGenRadius.GetText()->Render("Max critter radius",0x000000FF,14);
	lab_critMaxGenRadius.SetCenterX(false); lab_critMaxGenRadius.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMinGenGrowth);
	critMinGenGrowth.SetMaxLines(1); critMinGenGrowth.SetText(buff);
	lab_critMinGenGrowth.GetText()->Render("Min growth factor",0x000000FF,14);
	lab_critMinGenGrowth.SetCenterX(false); lab_critMinGenGrowth.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMaxGenGrowth);
	critMaxGenGrowth.SetMaxLines(1); critMaxGenGrowth.SetText(buff);
	lab_critMaxGenGrowth.GetText()->Render("Max growth factor",0x000000FF,14);
	lab_critMaxGenGrowth.SetCenterX(false); lab_critMaxGenGrowth.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMinGenSpeed);
	critMinGenSpeed.SetMaxLines(1); critMinGenSpeed.SetText(buff);
	lab_critMinGenSpeed.GetText()->Render("Min critter speed",0x000000FF,14);
	lab_critMinGenSpeed.SetCenterX(false); lab_critMinGenSpeed.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMaxGenSpeed);
	critMaxGenSpeed.SetMaxLines(1); critMaxGenSpeed.SetText(buff);
	lab_critMaxGenSpeed.GetText()->Render("Max critter speed",0x000000FF,14);
	lab_critMaxGenSpeed.SetCenterX(false); lab_critMaxGenSpeed.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMutateSize);
	critMutateSize.SetMaxLines(1); critMutateSize.SetText(buff);
	lab_critMutateSize.GetText()->Render("Mutation size",0x000000FF,14);
	lab_critMutateSize.SetCenterX(false); lab_critMutateSize.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMutateDiv);
	critMutateDiv.SetMaxLines(1); critMutateDiv.SetText(buff);
	lab_critMutateDiv.GetText()->Render("Mutation diversity",0x000000FF,14);
	lab_critMutateDiv.SetCenterX(false); lab_critMutateDiv.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMultiplierSpin);
	critMultiplierSpin.SetMaxLines(1); critMultiplierSpin.SetText(buff);
	lab_critMultiplierSpin.GetText()->Render("Spin multiplier",0x000000FF,14);
	lab_critMultiplierSpin.SetCenterX(false); lab_critMultiplierSpin.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMultiplierMoveForward);
	critMultiplierMoveForward.SetMaxLines(1); critMultiplierMoveForward.SetText(buff);
	lab_critMultiplierMoveForward.GetText()->Render("Forward multiplier",0x000000FF,14);
	lab_critMultiplierMoveForward.SetCenterX(false); lab_critMultiplierMoveForward.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMultiplierMoveBackward);
	critMultiplierMoveBackward.SetMaxLines(1); critMultiplierMoveBackward.SetText(buff);
	lab_critMultiplierMoveBackward.GetText()->Render("Backward multiplier",0x000000FF,14);
	lab_critMultiplierMoveBackward.SetCenterX(false); lab_critMultiplierMoveBackward.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyBirthFromParents);
	critEnergyBirthFromParents.SetMaxLines(1); critEnergyBirthFromParents.SetText(buff);
	lab_critEnergyBirthFromParents.GetText()->Render("Parent energy birth take",0x000000FF,14);
	lab_critEnergyBirthFromParents.SetCenterX(false); lab_critEnergyBirthFromParents.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMaxProccessedEnergy);
	critMaxProccessedEnergy.SetMaxLines(1); critMaxProccessedEnergy.SetText(buff);
	lab_critMaxProccessedEnergy.GetText()->Render("Max processed energy",0x000000FF,14);
	lab_critMaxProccessedEnergy.SetCenterX(false); lab_critMaxProccessedEnergy.SetCenterY(true);

	sprintf(buff,"%.324g",params->critMoodThershold);
	critMoodThershold.SetMaxLines(1); critMoodThershold.SetText(buff);
	lab_critMoodThershold.GetText()->Render("Mood therhold",0x000000FF,14);
	lab_critMoodThershold.SetCenterX(false); lab_critMoodThershold.SetCenterY(true);

	sprintf(buff,"%.324g",params->critAttackEnergyTake);
	critAttackEnergyTake.SetMaxLines(1); critAttackEnergyTake.SetText(buff);
	lab_critAttackEnergyTake.GetText()->Render("Attack energy take",0x000000FF,14);
	lab_critAttackEnergyTake.SetCenterX(false); lab_critAttackEnergyTake.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyMinToGiveBirth);
	critEnergyMinToGiveBirth.SetMaxLines(1); critEnergyMinToGiveBirth.SetText(buff);
	lab_critEnergyMinToGiveBirth.GetText()->Render("Min energy to breed",0x000000FF,14);
	lab_critEnergyMinToGiveBirth.SetCenterX(false); lab_critEnergyMinToGiveBirth.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyMinProcessedToGiveBirth);
	critEnergyMinProcessedToGiveBirth.SetMaxLines(1); critEnergyMinProcessedToGiveBirth.SetText(buff);
	lab_critEnergyMinProcessedToGiveBirth.GetText()->Render("Min processed to breed",0x000000FF,14);
	lab_critEnergyMinProcessedToGiveBirth.SetCenterX(false); lab_critEnergyMinProcessedToGiveBirth.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToLive);
	critEnergyToLive.SetMaxLines(1); critEnergyToLive.SetText(buff);
	lab_critEnergyToLive.GetText()->Render("Energy live take",0x000000FF,14);
	lab_critEnergyToLive.SetCenterX(false); lab_critEnergyToLive.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToMoveForw);
	critEnergyToMoveForw.SetMaxLines(1); critEnergyToMoveForw.SetText(buff);
	lab_critEnergyToMoveForw.GetText()->Render("Energy forward take",0x000000FF,14);
	lab_critEnergyToMoveForw.SetCenterX(false); lab_critEnergyToMoveForw.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToMoveBack);
	critEnergyToMoveBack.SetMaxLines(1); critEnergyToMoveBack.SetText(buff);
	lab_critEnergyToMoveBack.GetText()->Render("Energy backward take",0x000000FF,14);
	lab_critEnergyToMoveBack.SetCenterX(false); lab_critEnergyToMoveBack.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToChangeMove);
	critEnergyToChangeMove.SetMaxLines(1); critEnergyToChangeMove.SetText(buff);
	lab_critEnergyToChangeMove.GetText()->Render("Energy change move take",0x000000FF,14);
	lab_critEnergyToChangeMove.SetCenterX(false); lab_critEnergyToChangeMove.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToSpin);
	critEnergyToSpin.SetMaxLines(1); critEnergyToSpin.SetText(buff);
	lab_critEnergyToSpin.GetText()->Render("Energy spin take",0x000000FF,14);
	lab_critEnergyToSpin.SetCenterX(false); lab_critEnergyToSpin.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToChangeSpin);
	critEnergyToChangeSpin.SetMaxLines(1); critEnergyToChangeSpin.SetText(buff);
	lab_critEnergyToChangeSpin.GetText()->Render("Energy change spin take",0x000000FF,14);
	lab_critEnergyToChangeSpin.SetCenterX(false); lab_critEnergyToChangeSpin.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToMoodGood);
	critEnergyToMoodGood.SetMaxLines(1); critEnergyToMoodGood.SetText(buff);
	lab_critEnergyToMoodGood.GetText()->Render("Energy good mood take",0x000000FF,14);
	lab_critEnergyToMoodGood.SetCenterX(false); lab_critEnergyToMoodGood.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToMoodBad);
	critEnergyToMoodBad.SetMaxLines(1); critEnergyToMoodBad.SetText(buff);
	lab_critEnergyToMoodBad.GetText()->Render("Energy bad mood take",0x000000FF,14);
	lab_critEnergyToMoodBad.SetCenterX(false); lab_critEnergyToMoodBad.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToChangeMood);
	critEnergyToChangeMood.SetMaxLines(1); critEnergyToChangeMood.SetText(buff);
	lab_critEnergyToChangeMood.GetText()->Render("Max critter radius",0x000000FF,14);
	lab_critEnergyToChangeMood.SetCenterX(false); lab_critEnergyToChangeMood.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToExpose);
	critEnergyToExpose.SetMaxLines(1); critEnergyToExpose.SetText(buff);
	lab_critEnergyToExpose.GetText()->Render("Energy expose take",0x000000FF,14);
	lab_critEnergyToExpose.SetCenterX(false); lab_critEnergyToExpose.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToIncreaseExpose);
	critEnergyToIncreaseExpose.SetMaxLines(1); critEnergyToIncreaseExpose.SetText(buff);
	lab_critEnergyToIncreaseExpose.GetText()->Render("Energy + expose take",0x000000FF,14);
	lab_critEnergyToIncreaseExpose.SetCenterX(false); lab_critEnergyToIncreaseExpose.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToDecreaseExpose);
	critEnergyToDecreaseExpose.SetMaxLines(1); critEnergyToDecreaseExpose.SetText(buff);
	lab_critEnergyToDecreaseExpose.GetText()->Render("Energy - expose take",0x000000FF,14);
	lab_critEnergyToDecreaseExpose.SetCenterX(false); lab_critEnergyToDecreaseExpose.SetCenterY(true);

	sprintf(buff,"%.324g",params->critEnergyToBrain);
	critEnergyToBrain.SetMaxLines(1); critEnergyToBrain.SetText(buff);
	lab_critEnergyToBrain.GetText()->Render("Energy brain take",0x000000FF,14);
	lab_critEnergyToBrain.SetCenterX(false); lab_critEnergyToBrain.SetCenterY(true);
}

void Params::Set(Uint32 screen_x, Uint32 screen_y){
	scr_x = screen_x; scr_y = screen_y;
	Uint32 add = (-scrollPOS*30) + 50, spos = 0, maxY = screen_y-150;
	scrollUP.SetPos(scr_x-440,scr_y-135,scr_x-230,scr_y-115); scrollDOWN.SetPos(scr_x-220,scr_y-135,scr_x-10,scr_y-115);
	
	initialRandomAlg.SetPos(scr_x-230,add,scr_x-10,add+20); lab_initialRandomAlg.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		initialRandomAlg.Unregister(); lab_initialRandomAlg.Unregister();
	}else if(isRegistered){
		initialRandomAlg.Register(); lab_initialRandomAlg.Register();
	} add+=30; spos++;

	initialRandomSeed.SetPos(scr_x-230,add,scr_x-10,add+20); lab_initialRandomSeed.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		initialRandomSeed.Unregister(); lab_initialRandomSeed.Unregister();
	}else if(isRegistered){
		initialRandomSeed.Register(); lab_initialRandomSeed.Register();
	} add+=30; spos++;

	initialFoodCount.SetPos(scr_x-230,add,scr_x-10,add+20); lab_initialFoodCount.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		initialFoodCount.Unregister(); lab_initialFoodCount.Unregister();
	}else if(isRegistered){
		initialFoodCount.Register(); lab_initialFoodCount.Register();
	} add+=30; spos++;

	initialCritCount.SetPos(scr_x-230,add,scr_x-10,add+20); lab_initialCritCount.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		initialCritCount.Unregister(); lab_initialCritCount.Unregister();
	}else if(isRegistered){
		initialCritCount.Register(); lab_initialCritCount.Register();
	} add+=30; spos++;

	initialFoodDistribution.SetPos(scr_x-230,add,scr_x-10,add+20); lab_initialFoodDistribution.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		initialFoodDistribution.Unregister(); lab_initialFoodDistribution.Unregister();
	}else if(isRegistered){
		initialFoodDistribution.Register(); lab_initialFoodDistribution.Register();
	} add+=30; spos++;

	simRadius.SetPos(scr_x-230,add,scr_x-10,add+20); lab_simRadius.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		simRadius.Unregister(); lab_simRadius.Unregister();
	}else if(isRegistered){
		simRadius.Register(); lab_simRadius.Register();
	} add+=30; spos++;

	foodFriction.SetPos(scr_x-230,add,scr_x-10,add+20); lab_foodFriction.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		foodFriction.Unregister(); lab_foodFriction.Unregister();
	}else if(isRegistered){
		foodFriction.Register(); lab_foodFriction.Register();
	} add+=30; spos++;

	foodElastic.SetPos(scr_x-230,add,scr_x-10,add+20); lab_foodElastic.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		foodElastic.Unregister(); lab_foodElastic.Unregister();
	}else if(isRegistered){
		foodElastic.Register(); lab_foodElastic.Register();
	} add+=30; spos++;

	foodSpawnMomentum.SetPos(scr_x-230,add,scr_x-10,add+20); lab_foodSpawnMomentum.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		foodSpawnMomentum.Unregister(); lab_foodSpawnMomentum.Unregister();
	}else if(isRegistered){
		foodSpawnMomentum.Register(); lab_foodSpawnMomentum.Register();
	} add+=30; spos++;

	foodConvertAge.SetPos(scr_x-230,add,scr_x-10,add+20); lab_foodConvertAge.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		foodConvertAge.Unregister(); lab_foodConvertAge.Unregister();
	}else if(isRegistered){
		foodConvertAge.Register(); lab_foodConvertAge.Register();
	} add+=30; spos++;

	foodMaxInWorld.SetPos(scr_x-230,add,scr_x-10,add+20); lab_foodMaxInWorld.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		foodMaxInWorld.Unregister(); lab_foodMaxInWorld.Unregister();
	}else if(isRegistered){
		foodMaxInWorld.Register(); lab_foodMaxInWorld.Register();
	} add+=30; spos++;

	critMinInWorld.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMinInWorld.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMinInWorld.Unregister(); lab_critMinInWorld.Unregister();
	}else if(isRegistered){
		critMinInWorld.Register(); lab_critMinInWorld.Register();
	} add+=30; spos++;

	critMaxInWorld.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxInWorld.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxInWorld.Unregister(); lab_critMaxInWorld.Unregister();
	}else if(isRegistered){
		critMaxInWorld.Register(); lab_critMaxInWorld.Register();
	} add+=30; spos++;

	critMinGenNetSize.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMinGenNetSize.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMinGenNetSize.Unregister(); lab_critMinGenNetSize.Unregister();
	}else if(isRegistered){
		critMinGenNetSize.Register(); lab_critMinGenNetSize.Register();
	} add+=30; spos++;

	critMaxGenNetSize.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxGenNetSize.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxGenNetSize.Unregister(); lab_critMaxGenNetSize.Unregister();
	}else if(isRegistered){
		critMaxGenNetSize.Register(); lab_critMaxGenNetSize.Register();
	} add+=30; spos++;

	critMinGenRadius.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMinGenRadius.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMinGenRadius.Unregister(); lab_critMinGenRadius.Unregister();
	}else if(isRegistered){
		critMinGenRadius.Register(); lab_critMinGenRadius.Register();
	} add+=30; spos++;

	critMaxGenRadius.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxGenRadius.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxGenRadius.Unregister(); lab_critMaxGenRadius.Unregister();
	}else if(isRegistered){
		critMaxGenRadius.Register(); lab_critMaxGenRadius.Register();
	} add+=30; spos++;

	critMinGenGrowth.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMinGenGrowth.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMinGenGrowth.Unregister(); lab_critMinGenGrowth.Unregister();
	}else if(isRegistered){
		critMinGenGrowth.Register(); lab_critMinGenGrowth.Register();
	} add+=30; spos++;

	critMaxGenGrowth.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxGenGrowth.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxGenGrowth.Unregister(); lab_critMaxGenGrowth.Unregister();
	}else if(isRegistered){
		critMaxGenGrowth.Register(); lab_critMaxGenGrowth.Register();
	} add+=30; spos++;

	critMinGenSpeed.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMinGenSpeed.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMinGenSpeed.Unregister(); lab_critMinGenSpeed.Unregister();
	}else if(isRegistered){
		critMinGenSpeed.Register(); lab_critMinGenSpeed.Register();
	} add+=30; spos++;

	critMaxGenSpeed.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxGenSpeed.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxGenSpeed.Unregister(); lab_critMaxGenSpeed.Unregister();
	}else if(isRegistered){
		critMaxGenSpeed.Register(); lab_critMaxGenSpeed.Register();
	} add+=30; spos++;

	critMutateSize.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMutateSize.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMutateSize.Unregister(); lab_critMutateSize.Unregister();
	}else if(isRegistered){
		critMutateSize.Register(); lab_critMutateSize.Register();
	} add+=30; spos++;

	critMutateDiv.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMutateDiv.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMutateDiv.Unregister(); lab_critMutateDiv.Unregister();
	}else if(isRegistered){
		critMutateDiv.Register(); lab_critMutateDiv.Register();
	} add+=30; spos++;

	critMultiplierSpin.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMultiplierSpin.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMultiplierSpin.Unregister(); lab_critMultiplierSpin.Unregister();
	}else if(isRegistered){
		critMultiplierSpin.Register(); lab_critMultiplierSpin.Register();
	} add+=30; spos++;

	critMultiplierMoveForward.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMultiplierMoveForward.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMultiplierMoveForward.Unregister(); lab_critMultiplierMoveForward.Unregister();
	}else if(isRegistered){
		critMultiplierMoveForward.Register(); lab_critMultiplierMoveForward.Register();
	} add+=30; spos++;

	critMultiplierMoveBackward.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMultiplierMoveBackward.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMultiplierMoveBackward.Unregister(); lab_critMultiplierMoveBackward.Unregister();
	}else if(isRegistered){
		critMultiplierMoveBackward.Register(); lab_critMultiplierMoveBackward.Register();
	} add+=30; spos++;

	critEnergyBirthFromParents.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyBirthFromParents.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyBirthFromParents.Unregister(); lab_critEnergyBirthFromParents.Unregister();
	}else if(isRegistered){
		critEnergyBirthFromParents.Register(); lab_critEnergyBirthFromParents.Register();
	} add+=30; spos++;

	critMaxProccessedEnergy.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMaxProccessedEnergy.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMaxProccessedEnergy.Unregister(); lab_critMaxProccessedEnergy.Unregister();
	}else if(isRegistered){
		critMaxProccessedEnergy.Register(); lab_critMaxProccessedEnergy.Register();
	} add+=30; spos++;

	critMoodThershold.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critMoodThershold.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critMoodThershold.Unregister(); lab_critMoodThershold.Unregister();
	}else if(isRegistered){
		critMoodThershold.Register(); lab_critMoodThershold.Register();
	} add+=30; spos++;

	critAttackEnergyTake.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critAttackEnergyTake.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critAttackEnergyTake.Unregister(); lab_critAttackEnergyTake.Unregister();
	}else if(isRegistered){
		critAttackEnergyTake.Register(); lab_critAttackEnergyTake.Register();
	} add+=30; spos++;

	critEnergyMinToGiveBirth.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyMinToGiveBirth.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyMinToGiveBirth.Unregister(); lab_critEnergyMinToGiveBirth.Unregister();
	}else if(isRegistered){
		critEnergyMinToGiveBirth.Register(); lab_critEnergyMinToGiveBirth.Register();
	} add+=30; spos++;

	critEnergyMinProcessedToGiveBirth.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyMinProcessedToGiveBirth.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyMinProcessedToGiveBirth.Unregister(); lab_critEnergyMinProcessedToGiveBirth.Unregister();
	}else if(isRegistered){
		critEnergyMinProcessedToGiveBirth.Register(); lab_critEnergyMinProcessedToGiveBirth.Register();
	} add+=30; spos++;

	critEnergyToLive.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToLive.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToLive.Unregister(); lab_critEnergyToLive.Unregister();
	}else if(isRegistered){
		critEnergyToLive.Register(); lab_critEnergyToLive.Register();
	} add+=30; spos++;

	critEnergyToMoveForw.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToMoveForw.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToMoveForw.Unregister(); lab_critEnergyToMoveForw.Unregister();
	}else if(isRegistered){
		critEnergyToMoveForw.Register(); lab_critEnergyToMoveForw.Register();
	} add+=30; spos++;

	critEnergyToMoveBack.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToMoveBack.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToMoveBack.Unregister(); lab_critEnergyToMoveBack.Unregister();
	}else if(isRegistered){
		critEnergyToMoveBack.Register(); lab_critEnergyToMoveBack.Register();
	} add+=30; spos++;

	critEnergyToChangeMove.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToChangeMove.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToChangeMove.Unregister(); lab_critEnergyToChangeMove.Unregister();
	}else if(isRegistered){
		critEnergyToChangeMove.Register(); lab_critEnergyToChangeMove.Register();
	} add+=30; spos++;

	critEnergyToSpin.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToSpin.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToSpin.Unregister(); lab_critEnergyToSpin.Unregister();
	}else if(isRegistered){
		critEnergyToSpin.Register(); lab_critEnergyToSpin.Register();
	} add+=30; spos++;

	critEnergyToChangeSpin.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToChangeSpin.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToChangeSpin.Unregister(); lab_critEnergyToChangeSpin.Unregister();
	}else if(isRegistered){
		critEnergyToChangeSpin.Register(); lab_critEnergyToChangeSpin.Register();
	} add+=30; spos++;

	critEnergyToMoodGood.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToMoodGood.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToMoodGood.Unregister(); lab_critEnergyToMoodGood.Unregister();
	}else if(isRegistered){
		critEnergyToMoodGood.Register(); lab_critEnergyToMoodGood.Register();
	} add+=30; spos++;

	critEnergyToMoodBad.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToMoodBad.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToMoodBad.Unregister(); lab_critEnergyToMoodBad.Unregister();
	}else if(isRegistered){
		critEnergyToMoodBad.Register(); lab_critEnergyToMoodBad.Register();
	} add+=30; spos++;

	critEnergyToChangeMood.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToChangeMood.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToChangeMood.Unregister(); lab_critEnergyToChangeMood.Unregister();
	}else if(isRegistered){
		critEnergyToChangeMood.Register(); lab_critEnergyToChangeMood.Register();
	} add+=30; spos++;

	critEnergyToExpose.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToExpose.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToExpose.Unregister(); lab_critEnergyToExpose.Unregister();
	}else if(isRegistered){
		critEnergyToExpose.Register(); lab_critEnergyToExpose.Register();
	} add+=30; spos++;

	critEnergyToIncreaseExpose.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToIncreaseExpose.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToIncreaseExpose.Unregister(); lab_critEnergyToIncreaseExpose.Unregister();
	}else if(isRegistered){
		critEnergyToIncreaseExpose.Register(); lab_critEnergyToIncreaseExpose.Register();
	} add+=30; spos++;

	critEnergyToDecreaseExpose.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToDecreaseExpose.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToDecreaseExpose.Unregister(); lab_critEnergyToDecreaseExpose.Unregister();
	}else if(isRegistered){
		critEnergyToDecreaseExpose.Register(); lab_critEnergyToDecreaseExpose.Register();
	} add+=30; spos++;

	critEnergyToBrain.SetPos(scr_x-230,add,scr_x-10,add+20); lab_critEnergyToBrain.SetPos(scr_x-440,add,scr_x-230,add+20);
	if((spos<scrollPOS)||(add>maxY)){
		critEnergyToBrain.Unregister(); lab_critEnergyToBrain.Unregister();
	}else if(isRegistered){
		critEnergyToBrain.Register(); lab_critEnergyToBrain.Register();
	} add+=30; spos++;

}

void Params::Register(){
	scrollUP.Register(); scrollDOWN.Register(); isRegistered = true;
	Set(scr_x,scr_y);
}

void Params::Unregister(){
	scrollUP.Unregister(); scrollDOWN.Unregister(); isRegistered = false;
	initialRandomAlg.Unregister(); lab_initialRandomAlg.Unregister();
	initialRandomSeed.Unregister(); lab_initialRandomSeed.Unregister();
	initialFoodCount.Unregister(); lab_initialFoodCount.Unregister();
	initialCritCount.Unregister(); lab_initialCritCount.Unregister();
	initialFoodDistribution.Unregister(); lab_initialFoodDistribution.Unregister();
	simRadius.Unregister(); lab_simRadius.Unregister();
	foodFriction.Unregister(); lab_foodFriction.Unregister();
	foodElastic.Unregister(); lab_foodElastic.Unregister();
	foodSpawnMomentum.Unregister(); lab_foodSpawnMomentum.Unregister();
	foodConvertAge.Unregister(); lab_foodConvertAge.Unregister();
	foodMaxInWorld.Unregister(); lab_foodMaxInWorld.Unregister();
	critMinInWorld.Unregister(); lab_critMinInWorld.Unregister();
	critMaxInWorld.Unregister(); lab_critMaxInWorld.Unregister();
	critMinGenNetSize.Unregister(); lab_critMinGenNetSize.Unregister();
	critMaxGenNetSize.Unregister(); lab_critMaxGenNetSize.Unregister();
	critMinGenRadius.Unregister(); lab_critMinGenRadius.Unregister();
	critMaxGenRadius.Unregister(); lab_critMaxGenRadius.Unregister();

	critMinGenGrowth.Unregister(); lab_critMinGenGrowth.Unregister();
	critMaxGenGrowth.Unregister(); lab_critMaxGenGrowth.Unregister();
	critMinGenSpeed.Unregister(); lab_critMinGenSpeed.Unregister();
	critMaxGenSpeed.Unregister(); lab_critMaxGenSpeed.Unregister();
	critMutateSize.Unregister(); lab_critMutateSize.Unregister();
	critMutateDiv.Unregister(); lab_critMutateDiv.Unregister();
	critMultiplierSpin.Unregister(); lab_critMultiplierSpin.Unregister();
	critMultiplierMoveForward.Unregister(); lab_critMultiplierMoveForward.Unregister();
	critMultiplierMoveBackward.Unregister(); lab_critMultiplierMoveBackward.Unregister();
	critEnergyBirthFromParents.Unregister(); lab_critEnergyBirthFromParents.Unregister();
	critMaxProccessedEnergy.Unregister(); lab_critMaxProccessedEnergy.Unregister();
	critMoodThershold.Unregister(); lab_critMoodThershold.Unregister();
	critAttackEnergyTake.Unregister(); lab_critAttackEnergyTake.Unregister();
	critEnergyMinToGiveBirth.Unregister(); lab_critEnergyMinToGiveBirth.Unregister();
	critEnergyMinProcessedToGiveBirth.Unregister(); lab_critEnergyMinProcessedToGiveBirth.Unregister();
	critEnergyToLive.Unregister(); lab_critEnergyToLive.Unregister();
	critEnergyToMoveForw.Unregister(); lab_critEnergyToMoveForw.Unregister();
	critEnergyToMoveBack.Unregister(); lab_critEnergyToMoveBack.Unregister();
	critEnergyToChangeMove.Unregister(); lab_critEnergyToChangeMove.Unregister();
	critEnergyToSpin.Unregister(); lab_critEnergyToSpin.Unregister();
	critEnergyToChangeSpin.Unregister(); lab_critEnergyToChangeSpin.Unregister();
	critEnergyToMoodGood.Unregister(); lab_critEnergyToMoodGood.Unregister();
	critEnergyToMoodBad.Unregister(); lab_critEnergyToMoodBad.Unregister();
	critEnergyToChangeMood.Unregister(); lab_critEnergyToChangeMood.Unregister();
	critEnergyToExpose.Unregister(); lab_critEnergyToExpose.Unregister();
	critEnergyToIncreaseExpose.Unregister(); lab_critEnergyToIncreaseExpose.Unregister();
	critEnergyToDecreaseExpose.Unregister(); lab_critEnergyToDecreaseExpose.Unregister();
	critEnergyToBrain.Unregister(); lab_critEnergyToBrain.Unregister();
}

bool Params::Event(SDL_Event *event, SimulationParams *params){
	char buff[1024]; unsigned long int num = 0; double numD = 0.;
	if(event->type==SDL_USEREVENT){
		if(event->user.code==GUIBUTTON_RELEASE){
			if(event->user.data1==&scrollUP){
				if(scrollPOS>0)scrollPOS--;
				if(scrollPOS==0)scrollUP.SetDisabled(true);
				Set(scr_x,scr_y);
			}else if(event->user.data1==&scrollDOWN){
				scrollUP.SetDisabled(false);
				scrollPOS++;
				Set(scr_x,scr_y);
			}
		}else if(event->user.code==GUIDROPDOWN_SELECT){
			if(event->user.data1==&initialRandomAlg){
				switch(initialRandomAlg.GetSelected()){
				case 0: params->initialRandomAlg = RNDSTDLIB; break;
				case 1: params->initialRandomAlg = RNDLCG; break;
				case 2: params->initialRandomAlg = RNDMWC; break;
				case 3: params->initialRandomAlg = RNDWELL; break;
				case 4: params->initialRandomAlg = RNDMT; break;
				}
				return true;
			}
		}else if((event->user.code==GUITEXTBOX_RETURN)||(event->user.code==GUITEXTBOX_STOPTYPING)){
			if(event->user.data1==&initialRandomSeed){
				if(ToULong(initialRandomSeed.GetText(),&num))params->initialRandomSeed = num;
				sprintf(buff,"%u",params->initialRandomSeed);
				initialRandomSeed.SetText(buff);
				return true;
			}else if(event->user.data1==&initialFoodCount){
				if(ToULong(initialFoodCount.GetText(),&num))params->initialFoodCount = num;
				sprintf(buff,"%u",params->initialFoodCount);
				initialFoodCount.SetText(buff);
				return true;
			}else if(event->user.data1==&initialCritCount){
				if(ToULong(initialCritCount.GetText(),&num))params->initialCritCount = num;
				sprintf(buff,"%u",params->initialCritCount);
				initialCritCount.SetText(buff);
				return true;
			}else if(event->user.data1==&initialFoodDistribution){
				if(ToDouble(initialFoodDistribution.GetText(),&numD))params->initialFoodDistribution = numD;
				sprintf(buff,"%.324g",params->initialFoodDistribution);
				initialFoodDistribution.SetText(buff);
				return true;
			}else if(event->user.data1==&simRadius){
				if(ToDouble(simRadius.GetText(),&numD))params->simRadius = numD;
				sprintf(buff,"%.324g",params->simRadius);
				simRadius.SetText(buff);
				return true;
			}else if(event->user.data1==&foodFriction){
				if(ToDouble(foodFriction.GetText(),&numD))params->foodFriction = numD;
				sprintf(buff,"%.324g",params->foodFriction);
				foodFriction.SetText(buff);
				return true;
			}else if(event->user.data1==&foodElastic){
				if(ToDouble(foodElastic.GetText(),&numD))params->foodElastic = numD;
				sprintf(buff,"%.324g",params->foodElastic);
				foodElastic.SetText(buff);
				return true;
			}else if(event->user.data1==&foodSpawnMomentum){
				if(ToDouble(foodSpawnMomentum.GetText(),&numD))params->foodSpawnMomentum = numD;
				sprintf(buff,"%.324g",params->foodSpawnMomentum);
				foodSpawnMomentum.SetText(buff);
				return true;
			}else if(event->user.data1==&foodConvertAge){
				if(ToULong(foodConvertAge.GetText(),&num))params->foodConvertAge = num;
				sprintf(buff,"%u",params->foodConvertAge);
				foodConvertAge.SetText(buff);
				return true;
			}else if(event->user.data1==&foodMaxInWorld){
				if(ToULong(foodMaxInWorld.GetText(),&num))params->foodMaxInWorld = num;
				sprintf(buff,"%u",params->foodMaxInWorld);
				foodMaxInWorld.SetText(buff);
				return true;
			}else if(event->user.data1==&critMinInWorld){
				if(ToULong(critMinInWorld.GetText(),&num))params->critMinInWorld = num;
				sprintf(buff,"%u",params->critMinInWorld);
				critMinInWorld.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxInWorld){
				if(ToULong(critMaxInWorld.GetText(),&num))params->critMaxInWorld = num;
				sprintf(buff,"%u",params->critMaxInWorld);
				critMaxInWorld.SetText(buff);
				return true;
			}else if(event->user.data1==&critMinGenNetSize){
				if(ToULong(critMinGenNetSize.GetText(),&num))params->critMinGenNetSize = num;
				sprintf(buff,"%u",params->critMinGenNetSize);
				critMinGenNetSize.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxGenNetSize){
				if(ToULong(critMaxGenNetSize.GetText(),&num))params->critMaxGenNetSize = num;
				sprintf(buff,"%u",params->critMaxGenNetSize);
				critMaxGenNetSize.SetText(buff);
				return true;
			}else if(event->user.data1==&critMinGenRadius){
				if(ToDouble(critMinGenRadius.GetText(),&numD))params->critMinGenRadius = numD;
				sprintf(buff,"%.324g",params->critMinGenRadius);
				critMinGenRadius.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxGenRadius){
				if(ToDouble(critMaxGenRadius.GetText(),&numD))params->critMaxGenRadius = numD;
				sprintf(buff,"%.324g",params->critMaxGenRadius);
				critMaxGenRadius.SetText(buff);
				return true;
			}else if(event->user.data1==&critMinGenGrowth){
				if(ToDouble(critMinGenGrowth.GetText(),&numD))params->critMinGenGrowth = numD;
				sprintf(buff,"%.324g",params->critMinGenGrowth);
				critMinGenGrowth.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxGenGrowth){
				if(ToDouble(critMaxGenGrowth.GetText(),&numD))params->critMaxGenGrowth = numD;
				sprintf(buff,"%.324g",params->critMaxGenGrowth);
				critMaxGenGrowth.SetText(buff);
				return true;
			}else if(event->user.data1==&critMinGenSpeed){
				if(ToDouble(critMinGenSpeed.GetText(),&numD))params->critMinGenSpeed = numD;
				sprintf(buff,"%.324g",params->critMinGenSpeed);
				critMinGenSpeed.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxGenSpeed){
				if(ToDouble(critMaxGenSpeed.GetText(),&numD))params->critMaxGenSpeed = numD;
				sprintf(buff,"%.324g",params->critMaxGenSpeed);
				critMaxGenSpeed.SetText(buff);
				return true;
			}else if(event->user.data1==&critMutateSize){
				if(ToDouble(critMutateSize.GetText(),&numD))params->critMutateSize = numD;
				sprintf(buff,"%.324g",params->critMutateSize);
				critMutateSize.SetText(buff);
				return true;
			}else if(event->user.data1==&critMutateDiv){
				if(ToDouble(critMutateDiv.GetText(),&numD))params->critMutateDiv = numD;
				sprintf(buff,"%.324g",params->critMutateDiv);
				critMutateDiv.SetText(buff);
				return true;
			}else if(event->user.data1==&critMultiplierSpin){
				if(ToDouble(critMultiplierSpin.GetText(),&numD))params->critMultiplierSpin = numD;
				sprintf(buff,"%.324g",params->critMultiplierSpin);
				critMultiplierSpin.SetText(buff);
				return true;
			}else if(event->user.data1==&critMultiplierMoveForward){
				if(ToDouble(critMultiplierMoveForward.GetText(),&numD))params->critMultiplierMoveForward = numD;
				sprintf(buff,"%.324g",params->critMultiplierMoveForward);
				critMultiplierMoveForward.SetText(buff);
				return true;
			}else if(event->user.data1==&critMultiplierMoveBackward){
				if(ToDouble(critMultiplierMoveBackward.GetText(),&numD))params->critMultiplierMoveBackward = numD;
				sprintf(buff,"%.324g",params->critMultiplierMoveBackward);
				critMultiplierMoveBackward.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyBirthFromParents){
				if(ToDouble(critEnergyBirthFromParents.GetText(),&numD))params->critEnergyBirthFromParents = numD;
				sprintf(buff,"%.324g",params->critEnergyBirthFromParents);
				critEnergyBirthFromParents.SetText(buff);
				return true;
			}else if(event->user.data1==&critMaxProccessedEnergy){
				if(ToDouble(critMaxProccessedEnergy.GetText(),&numD))params->critMaxProccessedEnergy = numD;
				sprintf(buff,"%.324g",params->critMaxProccessedEnergy);
				critMaxProccessedEnergy.SetText(buff);
				return true;
			}else if(event->user.data1==&critMoodThershold){
				if(ToDouble(critMoodThershold.GetText(),&numD))params->critMoodThershold = numD;
				sprintf(buff,"%.324g",params->critMoodThershold);
				critMoodThershold.SetText(buff);
				return true;
			}else if(event->user.data1==&critAttackEnergyTake){
				if(ToDouble(critAttackEnergyTake.GetText(),&numD))params->critAttackEnergyTake = numD;
				sprintf(buff,"%.324g",params->critAttackEnergyTake);
				critAttackEnergyTake.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyMinToGiveBirth){
				if(ToDouble(critEnergyMinToGiveBirth.GetText(),&numD))params->critEnergyMinToGiveBirth = numD;
				sprintf(buff,"%.324g",params->critEnergyMinToGiveBirth);
				critEnergyMinToGiveBirth.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyMinProcessedToGiveBirth){
				if(ToDouble(critEnergyMinProcessedToGiveBirth.GetText(),&numD))params->critEnergyMinProcessedToGiveBirth = numD;
				sprintf(buff,"%.324g",params->critEnergyMinProcessedToGiveBirth);
				critEnergyMinProcessedToGiveBirth.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToLive){
				if(ToDouble(critEnergyToLive.GetText(),&numD))params->critEnergyToLive = numD;
				sprintf(buff,"%.324g",params->critEnergyToLive);
				critEnergyToLive.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToMoveForw){
				if(ToDouble(critEnergyToMoveForw.GetText(),&numD))params->critEnergyToMoveForw = numD;
				sprintf(buff,"%.324g",params->critEnergyToMoveForw);
				critEnergyToMoveForw.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToMoveBack){
				if(ToDouble(critEnergyToMoveBack.GetText(),&numD))params->critEnergyToMoveBack = numD;
				sprintf(buff,"%.324g",params->critEnergyToMoveBack);
				critEnergyToMoveBack.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToChangeMove){
				if(ToDouble(critEnergyToChangeMove.GetText(),&numD))params->critEnergyToChangeMove = numD;
				sprintf(buff,"%.324g",params->critEnergyToChangeMove);
				critEnergyToChangeMove.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToSpin){
				if(ToDouble(critEnergyToSpin.GetText(),&numD))params->critEnergyToSpin = numD;
				sprintf(buff,"%.324g",params->critEnergyToSpin);
				critEnergyToSpin.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToChangeSpin){
				if(ToDouble(critEnergyToChangeSpin.GetText(),&numD))params->critEnergyToChangeSpin = numD;
				sprintf(buff,"%.324g",params->critEnergyToChangeSpin);
				critEnergyToChangeSpin.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToMoodGood){
				if(ToDouble(critEnergyToMoodGood.GetText(),&numD))params->critEnergyToMoodGood = numD;
				sprintf(buff,"%.324g",params->critEnergyToMoodGood);
				critEnergyToMoodGood.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToMoodBad){
				if(ToDouble(critEnergyToMoodBad.GetText(),&numD))params->critEnergyToMoodBad = numD;
				sprintf(buff,"%.324g",params->critEnergyToMoodBad);
				critEnergyToMoodBad.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToChangeMood){
				if(ToDouble(critEnergyToChangeMood.GetText(),&numD))params->critEnergyToChangeMood = numD;
				sprintf(buff,"%.324g",params->critEnergyToChangeMood);
				critEnergyToChangeMood.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToExpose){
				if(ToDouble(critEnergyToExpose.GetText(),&numD))params->critEnergyToExpose = numD;
				sprintf(buff,"%.324g",params->critEnergyToExpose);
				critEnergyToExpose.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToIncreaseExpose){
				if(ToDouble(critEnergyToIncreaseExpose.GetText(),&numD))params->critEnergyToIncreaseExpose = numD;
				sprintf(buff,"%.324g",params->critEnergyToIncreaseExpose);
				critEnergyToIncreaseExpose.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToDecreaseExpose){
				if(ToDouble(critEnergyToDecreaseExpose.GetText(),&numD))params->critEnergyToDecreaseExpose = numD;
				sprintf(buff,"%.324g",params->critEnergyToDecreaseExpose);
				critEnergyToDecreaseExpose.SetText(buff);
				return true;
			}else if(event->user.data1==&critEnergyToBrain){
				if(ToDouble(critEnergyToBrain.GetText(),&numD))params->critEnergyToBrain = numD;
				sprintf(buff,"%.324g",params->critEnergyToBrain);
				critEnergyToBrain.SetText(buff);
				return true;
			}
		}
	}
	return false;
}

bool Params::ToLong(const char *str, long int *out){
	if((str==NULL)||(out==NULL))return false;
	while(isspace(*str))str++;
	if(*str==0)return false;
	char *endptr; long int val = 0;
	errno = 0; endptr = (char*)str; val = strtol(str,&endptr,10);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	errno = 0; endptr = (char*)str; val = strtol(str,&endptr,16);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	if(str[0]=='o'||str[0]=='O')str++;
	if(str[0]=='0'&&(str[0]=='o'||str[0]=='O')){ str+=2; }
	errno = 0; endptr = (char*)str; val = strtol(str,&endptr,8);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	errno = 0; endptr = (char*)str; val = strtol(str,&endptr,2);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	return false;
}

bool Params::ToULong(const char *str, long unsigned int *out){
	if((str==NULL)||(out==NULL))return false;
	while(isspace(*str))str++;
	if(*str==0)return false;
	char *endptr; long unsigned int val = 0;
	errno = 0; endptr = (char*)str; val = strtoul(str,&endptr,10);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	errno = 0; endptr = (char*)str; val = strtoul(str,&endptr,16);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	if(str[0]=='o'||str[0]=='O')str++;
	if(str[0]=='0'&&(str[0]=='o'||str[0]=='O')){ str+=2; }
	errno = 0; endptr = (char*)str; val = strtoul(str,&endptr,8);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	errno = 0; endptr = (char*)str; val = strtoul(str,&endptr,2);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	return false;
}

bool Params::ToDouble(const char *str, double *out){
	if((str==NULL)||(out==NULL))return false;
	while(isspace(*str))str++;
	if(*str==0)return false;
	if(strstr(str,"#IND")!=NULL){ *out = -std::numeric_limits<double>::quiet_NaN(); return true; }
	if(strstr(str,"#QNAN")!=NULL){ *out = std::numeric_limits<double>::quiet_NaN(); return true; }
	if(strstr(str,"#INF")!=NULL){
		if(strstr(str,"-")!=NULL)*out = -std::numeric_limits<double>::infinity();
		else *out = std::numeric_limits<double>::infinity();
		return true;
	}
	char *endptr; double val = 0;
	errno = 0; endptr = (char*)str; val = strtod(str,&endptr);
	if((errno==0)&&(isspace(*endptr)||(*endptr==0))){ *out = val; return true; }
	return false;
}
