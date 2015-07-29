/* NeuralNet v1.1.4 Date: 2013-04-20 */

#pragma once
#ifndef _NEURALNETWORK_HPP_
#define _NEURALNETWORK_HPP_
#define _NEURALNETWORK_VER_ 114

#include "RandomGenerator.hpp"
//#include "ConfigFile.hpp" //include to enable
#include "SDL_gfxPrimitives.h" //include to enable
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#define NEURALNODE_DEFAULTVAL 0.f
#define NEURALNET_DEFAULTWEIGHT 0.f

#define NODEVAL_MIN -1.f
#define NODEVAL_MAX 1.f
#define NODEWEIGHT_MIN 0.f
#define NODEWEIGHT_MAX 1.f
#define BOUNDS(x,min,max) ((x<min)?min:((x>max)?max:x))
#define BOUNDSWEIGHT(x) BOUNDS(x,NODEWEIGHT_MIN,NODEWEIGHT_MAX)
#define BOUNDSNODEVAL(x) BOUNDS(x,NODEVAL_MIN,NODEVAL_MAX)

class NeuralNode{
	float ma,mb,p; //params [-1,1]
	float sum,val;
public:
	friend class NeuralNet;
	NeuralNode();
	NeuralNode(float value);
	void Construct(); //default values
	void Activate(); //sum -> (activation with params) -> val
	void SetParams(float vma, float vmb, float vp);
	void SetParamsRandom(Random *rnd);
	void GetParams(float *vma, float *vmb, float *vp);
};

class NeuralNet{
	std::vector< std::vector<float> > weights; //weights[to][from]
	std::vector<NeuralNode> nodes; //nodes
	std::vector<unsigned int> nodeRnd;
	unsigned int nodeCount;
public:
	NeuralNet(); ~NeuralNet();
	NeuralNet& operator = (const NeuralNet &param); //copy everything

	void Step();
	void SetSize(unsigned int size);
	unsigned int GetSize();
	void Clear(); //SetSize(0);

	//node values
	void SetNode(unsigned int node, float value);
	void SetNodeAll(float value);
	void SetNodesRandom(Random *rnd, float min = NODEVAL_MIN, float max = NODEVAL_MAX);
	float GetNode(unsigned int node);

	//weights
	void SetWeight(unsigned int from, unsigned int to, float weight);
	void SetWeightAll(float weight);
	void SetWeightsRandom(Random *rnd, float min = NODEWEIGHT_MIN, float max = NODEWEIGHT_MAX);
	float GetWeight(unsigned int from, unsigned int to);

	//params
	void SetParams(unsigned int node, float ma, float mb, float p);
	void SetParamsAll(float ma, float mb, float p);
	void SetParamsRandom(Random *rnd);
	void GetParams(unsigned int node, float *ma, float *mb, float *p);

	//mutation
	void Mutate(Random *rnd, float size, float div);

	//crossover
	void Crossover(NeuralNet *A, NeuralNet *B);

	//import/export
	bool SaveToFile(const char *filename);
	bool LoadFromFile(const char *filename);

	#ifdef _CONFIGFILE_HPP_ //set/load from ConfigFile
	bool ConfigSet(ConfigFile *cfg, const char *prefix = NULL);
	bool ConfigLoad(ConfigFile *cfg, const char *prefix = NULL);
	#endif

	#ifdef _SDL_gfxPrimitives_h
	void Draw(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2);
	#endif

};

#endif
