#pragma once
#ifndef _SIMULATIONPARAMS_HPP_
#define _SIMULATIONPARAMS_HPP_

#include <stdint.h>
#include "RandomGenerator.hpp"

struct SimulationParams{

	RandAlg initialRandomAlg;
	uint32_t initialRandomSeed;
	uint32_t initialFoodCount, initialCritCount;
	float initialFoodDistribution; //[0,1] how much area total food takes (1 = whole canvas, 0 = nothing)

	Random rnd;
	float simRadius; //[0,INF] radius of the canvas
	float foodFriction; //[0,1] how much food slows down every frame (1 = no friction)
	float foodElastic; //[0,1] how elastic are collisions (1 = all enegry is conserved)
	float foodSpawnMomentum; //[0,INF] how much momentum does food have when it spawns
	uint32_t foodConvertAge; //how many frames it takes for the food to convert
	uint32_t foodMaxInWorld; //if number of food goes above this, it joins 2 food when they collide
	uint32_t critMinInWorld; //if number of critters drops below this, it chooses random parents and spawns new one
	uint32_t critMaxInWorld; //if number of critters goes above this, it kills a random one
	uint32_t critMinGenNetSize, critMaxGenNetSize; //limit netsize genome (0 < min < max)
	float critMinGenRadius, critMaxGenRadius; //limit radius genome (0 < min < max)
	float critMinGenGrowth, critMaxGenGrowth; //limit growth genome (0 < min < max < 1)
	float critMinGenSpeed, critMaxGenSpeed; //limit radius genome (0 < min < max < 1)
	float critMutateSize, critMutateDiv; //[0,1] mutation
	float critMultiplierSpin; //[0,1]
	float critMultiplierMoveForward, critMultiplierMoveBackward; //[0,1]
	float critEnergyBirthFromParents; //[0,1] how much energy to take from each parent
	float critMaxProccessedEnergy; //[0,INF] how much energy can be processed before a critter dies of old age (this*maxGenEnergy)
	float critMoodThershold;
	float critAttackEnergyTake; //[0,1]
	float critEnergyMinToGiveBirth; //[0,1] how much energy must a parent have in order to give birth
	float critEnergyMinProcessedToGiveBirth; //[0,INF] how much processed energy must there be in order to give birth
	float critEnergyToLive; //[0,1]
	float critEnergyToMoveForw, critEnergyToMoveBack, critEnergyToChangeMove;
	float critEnergyToSpin, critEnergyToChangeSpin;
	float critEnergyToMoodGood, critEnergyToMoodBad, critEnergyToChangeMood;
	float critEnergyToExpose, critEnergyToIncreaseExpose, critEnergyToDecreaseExpose;
	float critEnergyToBrain;

};

#endif
