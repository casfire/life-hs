#pragma once
#ifndef _CRITTER_HPP_
#define _CRITTER_HPP_

#include "simulationParams.hpp"
#include "RandomGenerator.hpp"
#include "NeuralNet.hpp"
#include "food.hpp"
#include "simulation.hpp"
#include "SDLHelper.hpp"
#include "MovableView.hpp"

#define CRITTER_NODES 23

class Food;
class Simulation;
class Critter{

	//genomes
	uint32_t gen_netSize; //[MIN,MAX] [0,INF] how many hidden neurons a critter has
	NeuralNet net;
	float gen_radius; //[MIN,MAX] [0,INF] max radius it can grow to
	float gen_growth; //[MIN,MAX] [0,1] how fast it grows (it never grows to gen_radius)
	float gen_speed; //[MIN,MAX] [0,1] max speed it can travel at (bigger values take more energy)

	//outputs
	float out_move; //[-1,1] -1=backwards, 1=forward
	float out_spin; //[-1,1] -1=negative, 1=positive
	float out_mood; //[-1,1] -1=attack, 1=mate
	float out_expose; //[-1,1] membrane radius = (out_expose+1)/2

	//inputs
	float in_random; //[-1,1]
	float in_size; //[0,1] 0 = zero, 1 = gen_radius
	float in_energy; //[0,1] 0 = death by hunger, 0.5 = hungry, 1 = full
	float in_waste; //[0,1] 0 = no waste, 1 = body full of waste
	float in_age; //[0,1] 0 = born, 1 = death by spending too much energy
	float in_foodNearAng; //[-1,1] nearest food direction
	float in_foodNearDis; //[0,1] nearest food distance
	float in_foodAng; //[-1,1] food smell direction
	float in_critNearAng; //[-1,1] nearest critter direction
	float in_critNearDis; //[0,1] nearest critter distance
	float in_critAng; //[-1,1] crit smell direction
	float in_sensorU, in_sensorUR, in_sensorR, in_sensorDR, in_sensorD, in_sensorDL, in_sensorL, in_sensorUL; //[0,1] critter mood sensors

	//physical
	float x, y, rot, rad, mrad, mradPow;
	float energy, waste; //[0,maxEnergy]
	float maxEnergy, maxGenEnergy; //PI*rad^2
	float processedEnergy;

	//temp
	float tmp_foodX, tmp_foodY, tmp_foodClosestDistance; Food *tmp_foodClosest;
	float tmp_critX, tmp_critY, tmp_critClosestDistance; Critter *tmp_critClosest;
	float tmp_exp;

	//sensor positions
	float tmp_sensorUx, tmp_sensorUy, tmp_sensorURx, tmp_sensorURy;
	float tmp_sensorRx, tmp_sensorRy, tmp_sensorDRx, tmp_sensorDRy;
	float tmp_sensorDx, tmp_sensorDy, tmp_sensorDLx, tmp_sensorDLy;
	float tmp_sensorLx, tmp_sensorLy, tmp_sensorULx, tmp_sensorULy;

public:
	bool active;
	friend class Simulation;
	Critter(); ~Critter();

	void Spawn(float posX, float posY, SimulationParams *params); //gives free energy
	void Spawn(float posX, float posY, Critter *parentA, Critter *parentB, SimulationParams *params);
	bool Frame(Simulation *simulation); //returns true on death
	void Interact(Critter *crit, Simulation *simulation);
	bool Interact(Food *food, SimulationParams *params); //returns true if eaten
	void Draw(SDL_Surface *dst, MovableView *view, bool selected);
	void Draw(SDL_Surface *dst, float xPos, float yPos, float dRad);

	bool Save(unsigned int n);
	bool Load(unsigned int n);

};

#endif
