#pragma once
#ifndef _PARAMS_HPP_
#define _PARAMS_HPP_

#include "SDLGui.hpp"
#include "simulation.hpp"
#include <limits>
#include <errno.h>

class Params{
	GUIButton scrollUP, scrollDOWN; Uint32 scrollPOS, scr_x, scr_y; bool isRegistered;
	GUIDropDown initialRandomAlg; GUILabel lab_initialRandomAlg;
	GUITextBox initialRandomSeed; GUILabel lab_initialRandomSeed;
	GUITextBox initialFoodCount; GUILabel lab_initialFoodCount;
	GUITextBox initialCritCount; GUILabel lab_initialCritCount;
	GUITextBox initialFoodDistribution; GUILabel lab_initialFoodDistribution;

	GUITextBox simRadius; GUILabel lab_simRadius;
	GUITextBox foodFriction; GUILabel lab_foodFriction;
	GUITextBox foodElastic; GUILabel lab_foodElastic;
	GUITextBox foodSpawnMomentum; GUILabel lab_foodSpawnMomentum;
	GUITextBox foodConvertAge; GUILabel lab_foodConvertAge;
	GUITextBox foodMaxInWorld; GUILabel lab_foodMaxInWorld;
	GUITextBox critMinInWorld; GUILabel lab_critMinInWorld;
	GUITextBox critMaxInWorld; GUILabel lab_critMaxInWorld;
	GUITextBox critMinGenNetSize; GUILabel lab_critMinGenNetSize;
	GUITextBox critMaxGenNetSize; GUILabel lab_critMaxGenNetSize;
	GUITextBox critMinGenRadius; GUILabel lab_critMinGenRadius;
	GUITextBox critMaxGenRadius; GUILabel lab_critMaxGenRadius;
	
	GUITextBox critMinGenGrowth; GUILabel lab_critMinGenGrowth;
	GUITextBox critMaxGenGrowth; GUILabel lab_critMaxGenGrowth;
	GUITextBox critMinGenSpeed; GUILabel lab_critMinGenSpeed;
	GUITextBox critMaxGenSpeed; GUILabel lab_critMaxGenSpeed;
	GUITextBox critMutateSize; GUILabel lab_critMutateSize;
	GUITextBox critMutateDiv; GUILabel lab_critMutateDiv;
	GUITextBox critMultiplierSpin; GUILabel lab_critMultiplierSpin;
	GUITextBox critMultiplierMoveForward; GUILabel lab_critMultiplierMoveForward;
	GUITextBox critMultiplierMoveBackward; GUILabel lab_critMultiplierMoveBackward;
	GUITextBox critEnergyBirthFromParents; GUILabel lab_critEnergyBirthFromParents;
	GUITextBox critMaxProccessedEnergy; GUILabel lab_critMaxProccessedEnergy;
	GUITextBox critMoodThershold; GUILabel lab_critMoodThershold;
	GUITextBox critAttackEnergyTake; GUILabel lab_critAttackEnergyTake;
	GUITextBox critEnergyMinToGiveBirth; GUILabel lab_critEnergyMinToGiveBirth;
	GUITextBox critEnergyMinProcessedToGiveBirth; GUILabel lab_critEnergyMinProcessedToGiveBirth;
	GUITextBox critEnergyToLive; GUILabel lab_critEnergyToLive;
	GUITextBox critEnergyToMoveForw; GUILabel lab_critEnergyToMoveForw;
	GUITextBox critEnergyToMoveBack; GUILabel lab_critEnergyToMoveBack;
	GUITextBox critEnergyToChangeMove; GUILabel lab_critEnergyToChangeMove;
	GUITextBox critEnergyToSpin; GUILabel lab_critEnergyToSpin;
	GUITextBox critEnergyToChangeSpin; GUILabel lab_critEnergyToChangeSpin;
	GUITextBox critEnergyToMoodGood; GUILabel lab_critEnergyToMoodGood;
	GUITextBox critEnergyToMoodBad; GUILabel lab_critEnergyToMoodBad;
	GUITextBox critEnergyToChangeMood; GUILabel lab_critEnergyToChangeMood;
	GUITextBox critEnergyToExpose; GUILabel lab_critEnergyToExpose;
	GUITextBox critEnergyToIncreaseExpose; GUILabel lab_critEnergyToIncreaseExpose;
	GUITextBox critEnergyToDecreaseExpose; GUILabel lab_critEnergyToDecreaseExpose;
	GUITextBox critEnergyToBrain; GUILabel lab_critEnergyToBrain;

	bool ToLong(const char *str, long int *out);
	bool ToULong(const char *str, long unsigned int *out);
	bool ToDouble(const char *str, double *out);
public:
	Params(); ~Params();
	void Init(SimulationParams *params);
	void Set(Uint32 screen_x, Uint32 screen_y);
	void Register();
	void Unregister();
	bool Event(SDL_Event *event, SimulationParams *params);
};

#endif
