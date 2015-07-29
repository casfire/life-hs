#pragma once
#ifndef _SIMULATION_HPP_
#define _SIMULATION_HPP_

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "simulationParams.hpp"
#include "RandomGenerator.hpp"
#include "NeuralNet.hpp"
#include "food.hpp"
#include "critter.hpp"
#include <vector>
#include "MovableView.hpp"
#include "SDLGui.hpp"

class Food;
class Critter;
class Simulation{

	std::vector<Food> food;
	std::vector<Critter> critter;
	uint64_t frameCount;
	uint32_t foodCount, critCount;

	uint32_t critSelect; bool critSelected;
	char buffer[1024];
	GUIText brainTextTop, brainText, infoText;

public:
	SimulationParams params;

	Simulation(); ~Simulation();

	void AddFood(float x, float y, float energy, float waste);
	void AddFood(float x, float y, float energy, float waste, float dir);
	void RemoveFood(uint32_t i);
	void AddCritter(float x, float y); //gives free energy
	void AddCritter(float x, float y, Critter *parentA, Critter *parentB); //takes energy from parents
	void RemoveCritter(uint32_t i);

	bool Load();
	bool Save();
	void Clear();
	void SetDefaultParams();
	void Start();
	void Frame();
	void Draw(SDL_Surface *dst, MovableView *view);
	void DrawAxes(SDL_Surface *dst, MovableView *view);
	bool CenterSelected(MovableView *view, bool location, bool rotation);
	bool IsSelected();
	
	uint64_t GetFrameCount();
	uint32_t GetCritterCount();
	uint32_t GetFoodCount();
	float GetEnergy();
	bool Click(float x, float y);
	void DrawBrainGUI(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2);
	void DrawInfoGUI(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2);
};

#endif
