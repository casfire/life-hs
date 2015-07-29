#pragma once
#ifndef _FOOD_HPP_
#define _FOOD_HPP_

#include "simulationParams.hpp"
#include "RandomGenerator.hpp"
#include "critter.hpp"
#include "simulation.hpp"
#include "MovableView.hpp"

class Critter;
class Simulation;
class Food{
	float x, y, sx, sy;
	float rad, energy;
	uint32_t age; bool isFood;
public:
	bool active;
	friend class Critter;
	friend class Simulation;
	Food(); ~Food();

	void Spawn(float x, float y, float energy, float waste, float dir, SimulationParams *params); //energy+waste must not be zero
	void Frame(SimulationParams *params);
	void Interact(Food *food, SimulationParams *params);
	bool Join(Food *food, SimulationParams *params); //returns true if joined
	void Draw(SDL_Surface *dst, MovableView *view, SimulationParams *params);

	bool Save(unsigned int n);
	bool Load(unsigned int n);

};

#endif
